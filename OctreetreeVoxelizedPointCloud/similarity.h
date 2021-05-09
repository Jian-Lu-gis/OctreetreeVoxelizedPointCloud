#pragma once

double Tonimoto(double fu, double fh) {
	//return (fu*fh) / ((fu*fu) + (fh*fh) - (fu*fh));
	if (fh < fu) {
		return fh / fu;
	}
	else
	{
		return fu / fh;
	}

}
double Similarity(double& roughnessu, double& roughnessh, double& ru, double& rh, double& gu, double& gh, double& bu, double& bh, double& wroughness, double& wr, double& wg, double& wb) {
	return (wroughness*Tonimoto(roughnessu, roughnessh) + wr * Tonimoto(ru, rh) + wg * Tonimoto(gu, gh) + wb * Tonimoto(bu, bh)) / (wroughness + wr + wg + wb);
}
// 空间特征度量
void SpatialFeatureMetric(SpaceVector& A, SpaceVector& UnitNormalVector, double& dish, double& disv) {
	dish = abs(A.x*UnitNormalVector.x + A.y*UnitNormalVector.y + A.z*UnitNormalVector.z);
	disv = Q_rsqrt(A.x*A.x + A.y*A.x + A.z*A.z - dish* dish);
}

double Dis2Occurrence(double& disH, double& disV) {
	double res = (0.7 / (1 + disH)) + (0.3 / (1 + disV));
	return res;
}

// 直方图相似性度量
double modelMatch(std::vector<float>& h, std::vector<float>& u) {
	int length = h.size();
	double similarity = 0;
	for (int i = 0; i < length; i++) {
		similarity += Q_rsqrt(h[i] * u[i]);
	}
	return similarity;
}