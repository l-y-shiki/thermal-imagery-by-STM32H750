#include <stdio.h>

int main(void) {
	int n, k;
	printf("请输入数组大小：");
	scanf("%d", &n);

	int arr[n], temp[n];
	printf("请输入 %d 个数组元素：", n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &arr[i]);
	}

	printf("请输入右移的位数：");
	scanf("%d", &k);

	// 对右移位数进行归一化，防止k大于数组长度
	k = k % n;

	// 利用辅助数组，将每个元素移到新位置
	// 原数组中的第 i 个元素移动到新数组的 (i+k)%n 位置
	for (int i = 0; i < n; i++) {
		temp[(i + k) % n] = arr[i];
	}

	printf("循环右移后的数组为：");
	for (int i = 0; i < n; i++) {
		printf("%d ", temp[i]);
	}
	printf("\n");

	return 0;
}

