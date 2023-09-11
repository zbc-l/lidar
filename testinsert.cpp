#include <iostream>
#include <fstream>
#include <vector>
#include <random>

int main() {
	std::vector<uint16_t> ref;
	const int vector_size = 78750;

	// 设置随机数生成器
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint16_t> dis(0, 100);

	// 生成随机数并填充到vector中
	for (int i = 0; i < vector_size; ++i) {
		uint16_t random_number = dis(gen);
		ref.push_back(random_number);
	}
	//std::vector<uint16_t> ref(78750); // 假设 ref 是一个 uint16_t 的 vector，已经有数据填充了
	std::vector<uint16_t> azimuthref; // 假设 azimuthref 也是一个 uint16_t 的 vector

	// 首次插入 ref
	azimuthref.insert(azimuthref.end(), ref.begin(), ref.end());

	// 修改 ref 的内容，将所有为 0 的值修改为正整数
	for (size_t i = 0; i < ref.size(); ++i) {
		if (ref[i] == 10) {
			ref[i] = 42; // 假设将所有为 0 的值修改为 42
		}
	}

	// 再次插入修改后的 ref，确保深拷贝
	azimuthref.insert(azimuthref.end(), ref.begin(), ref.end());

	// 将 azimuthref 存储为二进制文件
	std::ofstream outfile("azimuthref.bin", std::ios::binary);
	outfile.write(reinterpret_cast<const char*>(azimuthref.data()), azimuthref.size() * sizeof(uint16_t));
	outfile.close();

	std::cout << "azimuthref 已保存为二进制文件。" << std::endl;

	std::vector<uint16_t> attribute(78750);
	std::vector<uint16_t> ref(78750);
	std::vector<int_8> diff(startattributeidx-attributestartidx);

	k = 0;
	j = 0;
	for (i = attributestartidx; i < startattributeidx; i++) {
		if 
		if (mask[i] == 1) {
			attributediff = attribute[i]^ref[i]
				if (abs(attributediff) < 256) {
					diff[j]= attributediff
				}
				else {
					diff[j] = attributediff >> 8;
					overstepposition.push_back(i);
					oversteplow8.push_back(static_cast<uint8_t>(attributediff));
				}
			j += 1;
		}
		ref[k] = attribute[i];
		k += 1;
	}



	return 0;
}

#include <iostream>
#include <vector>

void reconstructAttribute(std::vector<int8_t>& diff, const std::vector<size_t>& overstepposition, const std::vector<uint8_t>& oversteplow8,
	const std::vector<bool>& mask, std::vector<uint16_t>& ref, std::vector<uint16_t>& attribute) {
	size_t j = 0; // Counter for elements that satisfy the mask condition

	for (size_t i = 0; i < mask.size(); i++) {
		if (mask[i]) {
			int8_t diff_value = diff[j];

			if (overstepposition.size() > 0 && i == overstepposition.front()) {
				// Reconstruct the full 16-bit value from high 8 bits and low 8 bits
				int16_t reconstructed_value = (diff_value << 8) | oversteplow8.front();
				attribute[i] = static_cast<uint16_t>(reconstructed_value);
				overstepposition.erase(overstepposition.begin()); // Remove the processed position
				oversteplow8.erase(oversteplow8.begin()); // Remove the processed low 8 bits
			}
			else {
				// Use the low 8 bits from the reference frame and add the diff_value
				attribute[i] = ref[i] + diff_value;
			}
			j++; // Increment the counter for satisfying elements
		}
		else {
			// If the element doesn't satisfy the mask condition, use the reference frame value
			attribute[i] = ref[i];
		}
	}
}

int main() {
	// Example data
	std::vector<int8_t> diff; // Your diff data here
	std::vector<size_t> overstepposition; // Your overstepposition data here
	std::vector<uint8_t> oversteplow8; // Your oversteplow8 data here
	std::vector<bool> mask; // Your mask data here
	std::vector<uint16_t> ref; // Your reference frame data here
	std::vector<uint16_t> attribute(78750, 0); // Initialize attribute with zeros

	// Call the reconstruction function
	reconstructAttribute(diff, overstepposition, oversteplow8, mask, ref, attribute);

	// Now, the 'attribute' vector should contain the reconstructed values

	return 0;
}

#include <iostream>
#include <vector>

void computeDifference(std::vector<bool>& mask, std::vector<uint16_t>& ref, std::vector<uint16_t>& attribute,
	std::vector<int8_t>& diff, std::vector<size_t>& overstepposition, std::vector<uint8_t>& oversteplow8,
	size_t attributestart, size_t attributeend, int8_t threshold) {
	size_t j = 0; // Counter for elements that satisfy the mask condition

	for (size_t i = attributestart; i < attributeend; i++) {
		if (mask[i]) {
			int8_t attributediff = static_cast<int8_t>(attribute[i]) ^ static_cast<int8_t>(ref[i]);

			if (attributediff >= -threshold && attributediff <= threshold) {
				diff.push_back(attributediff);
			}
			else {
				overstepposition.push_back(i);
				oversteplow8.push_back(static_cast<uint8_t>(attributediff & 0xFF));
				diff.push_back(static_cast<int8_t>(attributediff >> 8));
			}
			j++; // Increment the counter for satisfying elements
		}
		ref[i] = attribute[i]; // Update the reference frame
	}
}

void reconstructAttribute(std::vector<bool>& mask, std::vector<uint16_t>& ref, std::vector<uint16_t>& attribute,
	std::vector<int8_t>& diff, const std::vector<size_t>& overstepposition,
	const std::vector<uint8_t>& oversteplow8, size_t attributestart, size_t attributeend) {
	size_t j = 0; // Counter for elements that satisfy the mask condition

	for (size_t i = attributestart; i < attributeend; i++) {
		if (mask[i]) {
			int8_t diff_value = diff[j];

			if (overstepposition.size() > 0 && i == overstepposition.front()) {
				// Reconstruct the full 16-bit value from high 8 bits and low 8 bits
				int16_t reconstructed_value = (diff_value << 8) | oversteplow8[j];
				attribute[i] = static_cast<uint16_t>(reconstructed_value);
				overstepposition.erase(overstepposition.begin()); // Remove the processed position
			}
			else {
				// Use the low 8 bits from the reference frame and add the diff_value
				attribute[i] = ref[i] + diff_value;
			}
			j++; // Increment the counter for satisfying elements
		}
		else {
			// If the element doesn't satisfy the mask condition, use the reference frame value
			attribute[i] = ref[i];
		}
	}
}

int main() {
	// Example data
	std::vector<bool> mask(78750, true); // Your mask data here
	std::vector<uint16_t> ref(78750, 0); // Your reference frame data here
	std::vector<uint16_t> attribute(78750, 0); // Your current frame (attribute) data here
	size_t attributestart = 1000; // Starting index
	size_t attributeend = 50000; // Ending index

	// Parameters for thresholding
	int8_t threshold = 256;

	// Initialize diff, overstepposition, and oversteplow8
	std::vector<int8_t> diff;
	std::vector<size_t> overstepposition;
	std::vector<uint8_t> oversteplow8;

	// Compute the difference
	computeDifference(mask, ref, attribute, diff, overstepposition, oversteplow8, attributestart, attributeend, threshold);

	// Reconstruct the attribute
	reconstructAttribute(mask, ref, attribute, diff, overstepposition, oversteplow8, attributestart, attributeend);

	// Now, the 'attribute' vector should contain the reconstructed values

	return 0;
}


#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>
#include <algorithm>

int main() {
	// Example data
	int start = 200; // Start index
	int end = 230;   // End index

	// Generate 'tt' based on known range [start, end]
	std::vector<uint8_t> tt(630 / 8, 0);

	for (int packetidx = start; packetidx <= end; packetidx++) {
		int a = /* assign a value here */;

		if (a == 2) {
			int bit_idx = packetidx / 8;
			tt[bit_idx] |= static_cast<uint8_t>(pow(2.0, packetidx % 8));
		}
	}

	// Extract the relevant portion to 'output'
	std::vector<uint8_t> output(tt.begin() + (start / 8), tt.begin() + ((end + 7) / 8));
	int extracted_start = start;
	int extracted_end = end;

	// Reconstruct 'tt' based on the extracted range [extracted_start, extracted_end]
	std::vector<uint8_t> recontt(630 / 8, 0);
	//std::copy(output.begin(), output.end(), recontt.begin() + (extracted_start / 8));
	std::copy(output.begin(), output.end(), recontt.begin() + (extracted_start / 8));

	// Identify which packets have 'a' equal to 2
	for (int packetidx = 0; packetidx < 630; packetidx++) {
		int group_idx = packetidx / 8;
		int bit_idx = packetidx % 8;

		bool hasAEqualTo2 = (recontt[group_idx] >> bit_idx) & 1;
		if (hasAEqualTo2) {
			std::cout << "Packet " << packetidx << " has a value of 2." << std::endl;
		}
	}

	return 0;
}


