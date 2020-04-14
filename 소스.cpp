#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_YEAR_DURATION 10	// �Ⱓ

// ����ü ����
typedef struct {
    char	name[20];		// �̸�
    char	sex;			// ���� M or F
    int		freq[MAX_YEAR_DURATION]; // ������ ��
} tName;

typedef struct {
    int		len;		// �迭�� ����� �̸��� ��
    int		capacity;	// �迭�� �뷮 (�迭�� ���� ������ �̸��� ��)
    tName	*data;	// �̸� �迭�� ������
} tNames;

// �Լ� ���� ����

// ������ �Է� ������ �о� �̸� ����(�̸�, ����, ��)�� �̸� ����ü�� ����
// �̹� ����ü�� �����ϴ�(�����) �̸��� �ش� ������ �󵵸� ����
// ���� ������ �̸��� ����ü�� �߰�
// ���ǻ���: ���� �̸��� ��/�� ���� ���� �� �����Ƿ�, �̸��� ������ �����ؾ� ��
// ���ǻ���: ����ü �迭�� ���� ����Ʈ(ordered list)�̾�� ��
// �̹� ������ �̸����� �˻��ϱ� ���� bsearch �Լ��� ���
// ���ο� �̸��� ������ �޸� ������ Ȯ���ϱ� ���� memmove �Լ��� �̿��Ͽ� �޸𸮿� ����� ������ ����
// names->capacity�� 2�辿 ����
void load_names(FILE *fp, int year_index, tNames *names);

// ����ü �迭�� ȭ�鿡 ���
void print_names(tNames *names, int num_year);

// bsearch�� ���� �� �Լ�
int compare(const void *n1, const void *n2);

// ����Ž�� �Լ�
// return value: key�� �߰ߵǴ� ���, �迭�� �ε���
//				key�� �߰ߵ��� �ʴ� ���, key�� ���ԵǾ�� �� �迭�� �ε���
int binary_search(const void *key, const void *base, size_t nmemb, size_t size, int(*compare)(const void *, const void *));



// �Լ� ����

// �̸� ����ü �ʱ�ȭ
// len�� 0����, capacity�� 1�� �ʱ�ȭ
// return : ����ü ������
tNames *create_names(void)
{
    tNames *pnames = (tNames *)malloc(sizeof(tNames));

    pnames->len = 0;
    pnames->capacity = 1;
    pnames->data = (tName *)malloc(pnames->capacity * sizeof(tName));
    return pnames;
}

// �̸� ����ü�� �Ҵ�� �޸𸮸� ����
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
        tName *n = (tName *)malloc(sizeof(tName)); // ����ü n �ʱ�ȭ
        for (int i = 0; i < 10; i++) {
            n->freq[i] = 0;
        }
        ret = fscanf(fp, "%s", temp);
        if (ret == EOF)
            break;

        char *ptr = strtok(temp, ",");      // "," ���ڸ� �������� ���ڿ��� �ڸ�, ������ ��ȯ
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
            ptr = strtok(NULL, ",");      // ���� ���ڿ��� �߶� �����͸� ��ȯ
        } // �� �۾��� ���� n���� �� ���� ������ ���

        if (names->len >= names->capacity) { // tNames �迭�� ũ�� ������
            names->capacity *= 2;
            names->data = (tName *)realloc(names->data, names->capacity * sizeof(tName));
        }

        //���⸦ �ڵ��ؾ��� �� (tNames�� ��������� ���ʿ� ���ĵǵ��� �����ؾ���)
        tName_index = binary_search(n, names->data, names->len, sizeof(tName), compare);
        if (!strcmp(names->data[tName_index].name, n->name) && (names->data[tName_index].sex == n->sex))
            names->data[tName_index].freq[year_index] = n->freq[year_index];
        else {
            tName* dataPtr = names->data;
            memmove(dataPtr + (tName_index + 1), dataPtr + tName_index, (names->len - tName_index)*sizeof(tName)); // �޸𸮸� ��ĭ �а�,
            memcpy(dataPtr + tName_index, n, sizeof(tName)); // �ش� �ε����� ����
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

    if (b_result && (b_result->sex == ((tName*)key)->sex))// �̸��� �����ϰ�, �������� ���ٸ�
        return b_result - (tName*)base; // �� �̸��� �����ϴ� �ε��� ��ȯ

    else if (nmemb > 0) { // ����ü �迭�� ���Ұ� �ְ�, �̸��� ���ٸ�
        while (start <= end) { // ���� Ž������ �ڸ��� ã��
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
        return start;   // �ε����� ��ȯ
    }
    // �迭�� ���Ұ� ���ٸ� 
    return 0; // 0 ��ȯ
}


////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    tNames *names;

    int num = 0;
    FILE *fp;
    int num_year = 0;

    if (argc == 1) return 0;

    // �̸� ����ü �ʱ�ȭ
    names = create_names();

    // ù ���� �˾Ƴ��� "yob2009.txt" -> 2009
    int start_year = atoi(&argv[1][3]);

    for (int i = 1; i < argc; i++)
    {
        num_year++;
        fp = fopen(argv[i], "r");
        assert(fp != NULL);

        int year = atoi(&argv[i][3]); // ex) "yob2009.txt" -> 2009

        fprintf(stderr, "Processing [%s]..\n", argv[i]);

        // ������ �Է� ����(�̸� ����)�� ����ü�� ����
        load_names(fp, year-start_year, names);

        fclose(fp);
    }

    // �̸� ����ü�� ȭ�鿡 ���
    print_names(names, num_year);

    // �̸� ����ü ����
    destroy_names( names);

    return 1;
}
