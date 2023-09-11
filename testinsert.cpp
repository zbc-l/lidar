#include <iostream>
#include <fstream>
#include <vector>
#include <random>

int main() {
	std::vector<uint16_t> ref;
	const int vector_size = 78750;

	// ���������������
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint16_t> dis(0, 100);

	// �������������䵽vector��
	for (int i = 0; i < vector_size; ++i) {
		uint16_t random_number = dis(gen);
		ref.push_back(random_number);
	}
	//std::vector<uint16_t> ref(78750); // ���� ref ��һ�� uint16_t �� vector���Ѿ������������
	std::vector<uint16_t> azimuthref; // ���� azimuthref Ҳ��һ�� uint16_t �� vector

	// �״β��� ref
	azimuthref.insert(azimuthref.end(), ref.begin(), ref.end());

	// �޸� ref �����ݣ�������Ϊ 0 ��ֵ�޸�Ϊ������
	for (size_t i = 0; i < ref.size(); ++i) {
		if (ref[i] == 10) {
			ref[i] = 42; // ���轫����Ϊ 0 ��ֵ�޸�Ϊ 42
		}
	}

	// �ٴβ����޸ĺ�� ref��ȷ�����
	azimuthref.insert(azimuthref.end(), ref.begin(), ref.end());

	// �� azimuthref �洢Ϊ�������ļ�
	std::ofstream outfile("azimuthref.bin", std::ios::binary);
	outfile.write(reinterpret_cast<const char*>(azimuthref.data()), azimuthref.size() * sizeof(uint16_t));
	outfile.close();

	std::cout << "azimuthref �ѱ���Ϊ�������ļ���" << std::endl;

	return 0;
}
