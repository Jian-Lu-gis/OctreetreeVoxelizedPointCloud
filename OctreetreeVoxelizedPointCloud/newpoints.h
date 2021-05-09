#pragma once
#ifndef _OCTREE_POINTS_
#define _OCTREE_POINTS_

#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <Miniball.hpp>
#include "util.h"

using std::vector;
using std::string;

class PtsInfo {
public:
	enum PropType { kPoint = 1, kNormal = 2,kFeature = 4,KFPFH=8,KRoughness=16,kLabel = 32 };
	static const int kPTypeNum = 6;
	static const char kMagicStr[16];

public:
	PtsInfo() { reset(); }
	void reset();
	bool check_format(string& msg) const;
	bool has_property(PropType ptype) const {
		return (content_flags_ & ptype) != 0;
	}

	int pt_num() const { return pt_num_; }
	int channel(PropType ptype) const;
	int ptr_dis(PropType ptype) const;
	int sizeof_points() const { return ptr_dis_[kPTypeNum]; }

	void set_pt_num(int num) { pt_num_ = num; }
	void set_channel(PropType ptype, const int ch);
	void set_ptr_dis();

protected:
	int property_index(PropType ptype) const;

protected:
	char magic_str_[16];
	int pt_num_;
	int content_flags_;
	int channels_[8];
	int ptr_dis_[8];
};

struct PointsData
{
	int npt;
	const float* pts;
	const float* normals;
	const float* seg;
	const float* features;
	const float* fpfh;
	const float* roughness;
	const float* labels;
};

struct PointsBounds
{
	float radius;
	float center[3];
};

class Points {
public:
	Points() : info_(nullptr), buffer_() {}
	bool is_empty() const { return info_ == nullptr || info_->pt_num() == 0; }

	// the pts must not be empty, the labels may be empty,
	// the normals & features must not be empty at the same time.
	bool set_points(const vector<float>& pts, const vector<float>& normals,
		const vector<float>& features = vector<float>(),
		const vector<float>& fpfh = vector<float>(),
		const vector<float>& roughness = vector<float>(),
		const vector<float>& labels = vector<float>());
	void set_points(vector<char>& data); // swap data and buffer_

	PointsData get_points_data() const;
	PointsBounds get_points_bounds() const;

	bool read_points(const string& filename);
	bool write_points(const string& filename) const;

	const PtsInfo& info() const { return *info_; }
	const float* ptr(PtsInfo::PropType ptype) const;
	float* mutable_ptr(PtsInfo::PropType ptype);

	void center_about(const float* center);
	void displace(const float dis);

protected:
	PtsInfo* info_;
	vector<char> buffer_;
};

#endif // _OCTREE_POINTS_



////////////////////////
const char PtsInfo::kMagicStr[16] = "_POINTS_1.0_";

void PtsInfo::reset() {
	memset(this, 0, sizeof(PtsInfo));
	strcpy(magic_str_, kMagicStr);
}

bool PtsInfo::check_format(string& msg) const {
	msg.clear();
	if (strcmp(kMagicStr, magic_str_) != 0) {
		msg += "The version of points format is not " + string(kMagicStr) + ".\n";
	}
	if (pt_num_ < 0) {
		msg += "The pt_num_ should be larger than 0.\n";
	}
	// todo: add more checks

	// the PtsInfo is valid when no error message is produced
	return msg.empty();
}

int PtsInfo::channel(PropType ptype) const {
	int i = property_index(ptype);
	if (!has_property(ptype)) return 0;
	return channels_[i];
}

void PtsInfo::set_channel(PropType ptype, const int ch) {
	// note: the channel and content_flags_ are consisent.
	// If channels_[i] != 0, then the i^th bit of content_flags_ is 1.
	int i = property_index(ptype);
	channels_[i] = ch;
	content_flags_ |= ptype;
}

int PtsInfo::ptr_dis(PropType ptype) const {
	int i = property_index(ptype);
	if (!has_property(ptype)) return -1;
	return ptr_dis_[i];
}

void PtsInfo::set_ptr_dis() {
	// the accumulated pointer displacement
	ptr_dis_[0] = sizeof(PtsInfo);
	for (int i = 1; i <= kPTypeNum; ++i) { // note the " <= " is used here
		ptr_dis_[i] = ptr_dis_[i - 1] + sizeof(float) * pt_num_ * channels_[i - 1];
	}
}

int PtsInfo::property_index(PropType ptype) const {
	int k = 0, p = ptype;
	for (int i = 0; i < kPTypeNum; ++i) {
		if (0 != (p & (1 << i))) {
			k = i; break;
		}
	}
	return k;
}

////////////////////////
bool Points::read_points(const string& filename) {
	std::ifstream infile(filename, std::ios::binary);
	if (!infile) return false;

	infile.seekg(0, infile.end);
	size_t len = infile.tellg();
	infile.seekg(0, infile.beg);

	buffer_.resize(len);
	infile.read(buffer_.data(), len);
	info_ = reinterpret_cast<PtsInfo*>(buffer_.data());

	infile.close();
	return true;
}

bool Points::write_points(const string& filename) const {
	std::ofstream outfile(filename, std::ios::binary);
	if (!outfile) return false;
	outfile.write(buffer_.data(), buffer_.size());
	outfile.close();
	return true;
}

const float* Points::ptr(PtsInfo::PropType ptype) const {
	const float* p = nullptr;
	int dis = info_->ptr_dis(ptype);
	if (-1 != dis) {
		p = reinterpret_cast<const float*>(buffer_.data() + dis);
	}
	return p;
}

float* Points::mutable_ptr(PtsInfo::PropType ptype) {
	return const_cast<float*>(ptr(ptype));
}

PointsData Points::get_points_data() const
{
	PointsData points_data;

	points_data.npt = info_->pt_num();
	points_data.pts = ptr(PtsInfo::kPoint);
	points_data.normals = ptr(PtsInfo::kNormal);
	points_data.features = ptr(PtsInfo::kFeature);
	points_data.fpfh = ptr(PtsInfo::KFPFH);
	points_data.roughness = ptr(PtsInfo::KRoughness);
	points_data.labels = ptr(PtsInfo::kLabel);

	return points_data;
}

PointsBounds Points::get_points_bounds() const
{
	int npt = info_->pt_num();
	const float* pts = ptr(PtsInfo::kPoint);

	PointsBounds bounds;
	bounding_sphere(bounds.radius, bounds.center, pts, npt);

	return bounds;
}
bool Points::set_points(const vector<float>& pts, const vector<float>& normals,
	const vector<float>& features, const vector<float>& fpfh, const vector<float>& roughness,const vector<float>& labels) {
	/// set info
	int num = pts.size() / 3;
	// !!! Empty input is not allowed
	if (num == 0) return false;
	PtsInfo info;
	info.set_pt_num(num);
	info.set_channel(PtsInfo::kPoint, 3);

	if (!normals.empty()) {
		int c = normals.size() / num;
		int r = normals.size() % num;
		// !!! The channel of normal has to be 3
		if (3 != c || 0 != r) return false;
		info.set_channel(PtsInfo::kNormal, c);
	}

	if (!features.empty()) {
		int c = features.size() / num;
		int r = features.size() % num;
		// !!! The channel of normal has to larger than 0
		if (0 == c || 0 != r) return false;
		info.set_channel(PtsInfo::kFeature, c);
	}
	if (!fpfh.empty()) {
		int c = fpfh.size() / num;
		int r = fpfh.size() % num;
		// !!! The channel of normal has to larger than 0
		if (0 == c || 0 != r) return false;
		info.set_channel(PtsInfo::KFPFH, c);
	}
	if (!roughness.empty()) {
		int c = roughness.size() / num;
		int r = roughness.size() % num;
		// !!! The channel of normal has to larger than 0
		if (0 == c || 0 != r) return false;
		info.set_channel(PtsInfo::KRoughness, c);
	}

	if (!labels.empty()) {
		int c = labels.size() / num;
		int r = labels.size() % num;
		// !!! The channel of label has to be 1
		if (1 != c || 0 != r) return false;
		info.set_channel(PtsInfo::kLabel, c);
	}

	info.set_ptr_dis();

	/// set buffer
	int sz = info.sizeof_points();
	buffer_.resize(sz);
	memcpy(buffer_.data(), &info, sizeof(PtsInfo));
	info_ = reinterpret_cast<PtsInfo*>(buffer_.data());
	copy(pts.begin(), pts.end(), mutable_ptr(PtsInfo::kPoint));
	if (!normals.empty()) {
		copy(normals.begin(), normals.end(), mutable_ptr(PtsInfo::kNormal));
	}
	if (!features.empty()) {
		copy(features.begin(), features.end(), mutable_ptr(PtsInfo::kFeature));
	}
	if (!fpfh.empty()) {
		copy(fpfh.begin(), fpfh.end(), mutable_ptr(PtsInfo::KFPFH));
	}
	if (!roughness.empty()) {
		copy(roughness.begin(), roughness.end(), mutable_ptr(PtsInfo::KRoughness));
	}
	if (!labels.empty()) {
		copy(labels.begin(), labels.end(), mutable_ptr(PtsInfo::kLabel));
	}

	return true;
}

void Points::set_points(vector<char>& data) {
	buffer_.swap(data);
	info_ = reinterpret_cast<PtsInfo*>(buffer_.data());
}

void Points::center_about(const float* center) {
	const int dim = 3, npt = info_->pt_num();
	float* pt = mutable_ptr(PtsInfo::kPoint);
	for (int i = 0; i < npt; ++i) {
		int i3 = i * 3;
		for (int m = 0; m < dim; ++m) {
			pt[i3 + m] -= center[m];
		}
	}
}

void Points::displace(const float dis) {
	const int dim = 3, npt = info_->pt_num();
	float* pt = mutable_ptr(PtsInfo::kPoint);
	float* normal = mutable_ptr(PtsInfo::kNormal);
	if (normal == nullptr) return;

	for (int i = 0; i < npt; ++i) {
		int i3 = i * 3;
		for (int m = 0; m < 3; ++m) {
			pt[i3 + m] += normal[i3 + m] * dis;
		}
	}
}
