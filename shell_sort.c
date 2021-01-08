#include <stdio.h>


#define DATA_ARRAY_LENGTH       12


int shell_sort(int *data, int length) 
{
	int gap = 0; //分组的跨度
	int i = 0, j = 0;

	// 分组的次数
	for(gap = length / 2;gap >= 1;gap /= 2){ 	
		// 每组遍历
		for(i = gap;i < length;i++){ 			 
            int temp = data[i];
            //组内排序
            for (j = i - gap; j >= 0 && temp < data[j];j = j - gap){ 
                data[j+gap] = data[j];
            }
            data[j+gap] = temp;
        }
    }
    return 0;
}

int main() {

    int i = 0;
    int data[DATA_ARRAY_LENGTH] = {23, 64, 24, 12, 9, 16, 53, 57, 71, 79, 87, 97};
	int len = sizeof(data) / sizeof(int);
    shell_sort(data, len);

    for(i = 0;i < len;i ++){
		printf("%4d", data[i]);
    }
    printf("\n");
}
