#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_YEAR_DURATION 10	// 기간

// 구조체 선언
typedef struct {
    char	name[20];		// 이름
    char	sex;			// 성별 M or F
    int		freq[MAX_YEAR_DURATION]; // 연도별 빈도
} tName;

typedef struct {
    int		len;		// 배열에 저장된 이름의 수
    int		capacity;	// 배열의 용량 (배열에 저장 가능한 이름의 수)
    tName	*data;	// 이름 배열의 포인터
} tNames;

// 함수 원형 선언

// 연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// 주의사항: 구조체 배열은 정렬 리스트(ordered list)이어야 함
// 이미 등장한 이름인지 검사하기 위해 bsearch 함수를 사용
// 새로운 이름을 저장할 메모리 공간을 확보하기 위해 memmove 함수를 이용하여 메모리에 저장된 내용을 복사
// names->capacity는 2배씩 증가
void load_names(FILE *fp, int year_index, tNames *names);

// 구조체 배열을 화면에 출력
void print_names(tNames *names, int num_year);

// bsearch를 위한 비교 함수
int compare(const void *n1, const void *n2);

// 이진탐색 함수
// return value: key가 발견되는 경우, 배열의 인덱스
//				key가 발견되지 않는 경우, key가 삽입되어야 할 배열의 인덱스
int binary_search(const void *key, const void *base, size_t nmemb, size_t size, int(*compare)(const void *, const void *));



// 함수 정의

// 이름 구조체 초기화
// len를 0으로, capacity를 1로 초기화
// return : 구조체 포인터
tNames *create_names(void)
{
    tNames *pnames = (tNames *)malloc(sizeof(tNames));

    pnames->len = 0;
    pnames->capacity = 1;
    pnames->data = (tName *)malloc(pnames->capacity * sizeof(tName));
    return pnames;
}

// 이름 구조체에 할당된 메모리를 해제
void destroy_names(tNames *pnames)
{
    free(pnames->data);
    pnames->len = 0;
    pnames->capacity = 0;

    free(pnames);
}

void load_names(FILE* fp, int year_index, tNames* names) {
    int ret; char temp[30];
    fopen("fp", "r");

    while (1) {
        int tName_index=0;
        tName *n = (tName *)malloc(sizeof(tName)); // 구조체 n 초기화
        for (int i = 0; i < 10; i++) {
            n->freq[i] = 0;
        }
        ret = fscanf(fp, "%s", temp);
        if (ret == EOF)
            break;

        char *ptr = strtok(temp, ",");      // "," 문자를 기준으로 문자열을 자름, 포인터 반환
        for (int i = 0; i<3; i++) {
            switch (i) {
            case 0:
                strcpy(n->name, ptr);
                break;
            case 1:
                n->sex = *ptr;
                break;
            case 2:
                n->freq[year_index] = atoi(ptr);
                break;
            default:
                break;
            }
            ptr = strtok(NULL, ",");      // 다음 문자열을 잘라서 포인터를 반환
        } // 이 작업을 통해 n에는 한 줄의 정보가 담김

        if (names->len >= names->capacity) { // tNames 배열의 크기 재지정
            names->capacity *= 2;
            names->data = (tName *)realloc(names->data, names->capacity * sizeof(tName));
        }

        //여기를 코딩해야함 ↓ (tNames에 집어넣을때 애초에 정렬되도록 삽입해야함)
        tName_index = binary_search(n, names->data, names->len, sizeof(tName), compare);
        if (!strcmp(names->data[tName_index].name, n->name) && (names->data[tName_index].sex == n->sex))
            names->data[tName_index].freq[year_index] = n->freq[year_index];
        else {
            tName* dataPtr = names->data;
            memmove(dataPtr + (tName_index + 1), dataPtr + tName_index, (names->len - tName_index)*sizeof(tName)); // 메모리를 한칸 밀고,
            memcpy(dataPtr + tName_index, n, sizeof(tName)); // 해당 인덱스에 삽입
            names->len++;
        }
        free(n);
    }
}

void print_names(tNames* names, int num_year) {
    tName* N = names->data;
    for (int i = 0; i < names->len; i++)
        printf("%s %c %d %d %d %d %d %d %d %d %d %d\n", N[i].name, N[i].sex, N[i].freq[0], N[i].freq[1], N[i].freq[2], N[i].freq[3], N[i].freq[4], N[i].freq[5], N[i].freq[6], N[i].freq[7], N[i].freq[8], N[i].freq[9]);
}

int compare(const void *n1, const void *n2) {
    return strcmp(((tName*)n1)->name, ((tName*)n2)->name);
}

int binary_search(const void *key, const void *base, size_t nmemb, size_t size, int(*compare)(const void *, const void *)) {
    int start = 0, mid, end = nmemb - 1;
    tName* b_result = (tName*)bsearch(key, base, nmemb, size, compare);

    if (b_result && (b_result->sex == ((tName*)key)->sex))// 이름이 존재하고, 성별까지 같다면
        return b_result - (tName*)base; // 그 이름이 존재하는 인덱스 반환

    else if (nmemb > 0) { // 구조체 배열에 원소가 있고, 이름이 없다면
        while (start <= end) { // 이진 탐색으로 자리를 찾아
            mid = (start + end) / 2;
            int cmp = strcmp(((tName*)base)[mid].name, ((tName*)key)->name);
            if (cmp == 0) {
                if (((tName*)base)[mid].sex == ((tName*)key)->sex)
                    return mid;
                if (((tName*)base)[mid].sex == 'F')
                    return mid + 1;
                if (((tName*)base)[mid].sex == 'M') {
                    if (!strcmp(((tName*)base)[mid - 1].name, ((tName*)key)->name) && ((tName*)base)[mid - 1].sex == 'F')
                        return mid - 1;
                    else
                        return mid;
                }
            }
            else if (cmp == 1)
                end = mid - 1;
            else
                start = mid + 1;
        }
        return start;   // 인덱스를 반환
    }
    // 배열에 원소가 없다면 
    return 0; // 0 반환
}


////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    tNames *names;

    int num = 0;
    FILE *fp;
    int num_year = 0;

    if (argc == 1) return 0;

    // 이름 구조체 초기화
    names = create_names();

    // 첫 연도 알아내기 "yob2009.txt" -> 2009
    int start_year = atoi(&argv[1][3]);

    for (int i = 1; i < argc; i++)
    {
        num_year++;
        fp = fopen(argv[i], "r");
        assert(fp != NULL);

        int year = atoi(&argv[i][3]); // ex) "yob2009.txt" -> 2009

        fprintf(stderr, "Processing [%s]..\n", argv[i]);

        // 연도별 입력 파일(이름 정보)을 구조체에 저장
        load_names(fp, year-start_year, names);

        fclose(fp);
    }

    // 이름 구조체를 화면에 출력
    print_names(names, num_year);

    // 이름 구조체 해제
    destroy_names( names);

    return 1;
}
