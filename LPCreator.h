#pragma once
#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
namespace QualitativeClustering {
	void createLP(std::string dataset, std::string LPfilename, int modelType, int threshold=0) {
		
		int n, m;
		std::vector<std::vector<int>>S;

		std::ifstream finput;
		finput.open(dataset.c_str(), std::fstream::in);
		if (finput.is_open() != true) {
			std::cerr << "The file " << dataset << " does not exist" << std::endl;
			exit(0);
		}

		std::string value;
		std::string line;
		std::vector<std::vector<std::string>>D;
		while (getline(finput, line)) {
			std::istringstream colunas(line);
			std::vector<std::string> coluna;
			while (colunas >> value)
				coluna.push_back(value);
			D.push_back(coluna);
		}
		finput.close();

		n = D.size();
		m = D[0].size();

		// Create similarity matrix.
		int i, j, k;
		S.resize(n);
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				S[i].push_back(-m - 1);

		// Calculate the simetric difference between each instance and store it in 'S'.
		// Missing values "?" are ignored in the similarity calculus.
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (i != j) {
					int total = 0;
					int total_missing = 0;
					for (k = 0; k < m; k++) {
						if (D[i][k].compare("?") != 0 && D[j][k].compare("?") != 0) {
							if (D[i][k].compare(D[j][k]) == 0)
								total = total + 1;
						}
						else {
							total_missing = total_missing + 1;
						}
					}
					S[i][j] = 2 * total - (m - total_missing);
				}
			}
		}

		if (threshold > 0) {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					S[i][j] = S[i][j] - threshold;
				}
			}
		}

		////////////////////
		// Save LP file
		std::ofstream LP;
		LP.open(LPfilename.c_str()); LP.precision(std::numeric_limits<double>::max_digits10);
		LP << "Maximize\n";
		///////////////
		// Create objective function
		LP << "obj: ";
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				if (S[i][j] >= 0) {
					LP << " + " << std::fixed << S[i][j] << " x" << i + 1 << "." << j + 1 << " ";
				}
				else {
					LP << std::fixed << S[i][j] << " x" << i + 1 << "." << j + 1 << " ";
				}
			}
		}
		LP << std::endl;
		///////////////
		// Create constraints
		LP << "\nSubject To\n";
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				for (int k = j + 1; k < n; k++) {

					if (modelType == 0) {
						LP << "x" << i + 1 << "." << j + 1 << " + x" << j + 1 << "." << k + 1 << " - x" << i + 1 << "." << k + 1 << " <= 1" << std::endl;
						LP << "x" << i + 1 << "." << j + 1 << " - x" << j + 1 << "." << k + 1 << " + x" << i + 1 << "." << k + 1 << " <= 1" << std::endl;
						LP << "- x" << i + 1 << "." << j + 1 << " + x" << j + 1 << "." << k + 1 << " + x" << i + 1 << "." << k + 1 << " <= 1" << std::endl;
					}

					if (modelType == 1) {
						if (S[i][j] >= 0 || S[j][k] >= 0) {
							LP << "x" << i + 1 << "." << j + 1 << " + x" << j + 1 << "." << k + 1 << " - x" << i + 1 << "." << k + 1 << " <= 1" << std::endl;
						}
						if (S[i][j] >= 0 || S[i][k] >= 0) {
							LP << "x" << i + 1 << "." << j + 1 << " - x" << j + 1 << "." << k + 1 << " + x" << i + 1 << "." << k + 1 << " <= 1" << std::endl;
						}
						if (S[j][k] >= 0 || S[i][k] >= 0) {
							LP << "- x" << i + 1 << "." << j + 1 << " + x" << j + 1 << "." << k + 1 << " + x" << i + 1 << "." << k + 1 << " <= 1" << std::endl;
						}
					}

					if (modelType == 2) {
						if (S[i][j] + S[j][k] >= 0) {
							LP << "x" << i + 1 << "." << j + 1 << " + x" << j + 1 << "." << k + 1 << " - x" << i + 1 << "." << k + 1 << " <= 1" << std::endl;
						}
						if (S[i][j] + S[i][k] >= 0) {
							LP << "x" << i + 1 << "." << j + 1 << " - x" << j + 1 << "." << k + 1 << " + x" << i + 1 << "." << k + 1 << " <= 1" << std::endl;
						}
						if (S[j][k] + S[i][k] >= 0) {
							LP << "- x" << i + 1 << "." << j + 1 << " + x" << j + 1 << "." << k + 1 << " + x" << i + 1 << "." << k + 1 << " <= 1" << std::endl;
						}
					}

					if (modelType == 3) {
						if ((S[i][j] + S[j][k] >= 0) && S[i][j] >= 0 && S[i][k] <= 0) {
							LP << "x" << i + 1 << "." << j + 1 << " + x" << j + 1 << "." << k + 1 << " - x" << i + 1 << "." << k + 1 << " <= 1" << std::endl;
						}
						if ((S[i][j] + S[i][k] >= 0) && S[i][j] >= 0 && S[j][k] <= 0) {
							LP << "x" << i + 1 << "." << j + 1 << " - x" << j + 1 << "." << k + 1 << " + x" << i + 1 << "." << k + 1 << " <= 1" << std::endl;
						}
						if ((S[i][k] + S[j][k] >= 0) && S[i][k] >= 0 && S[i][j] <= 0) {
							LP << "- x" << i + 1 << "." << j + 1 << " + x" << j + 1 << "." << k + 1 << " + x" << i + 1 << "." << k + 1 << " <= 1" << std::endl;
						}
					}

				}
			}
		}

		///////////////////
		// Variables Bounds
		LP << "Bounds\n";
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				LP << 0 << " <= x" << i + 1 << "." << j + 1 << " <= 1" << std::endl;
			}
		}

		///////////////////
		// Variables Types
		LP << "Binaries\n";
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				LP << "x" << i + 1 << "." << j + 1 << std::endl;
			}
		}

		LP << std::endl << "End\n";

		LP.close();
	}
}