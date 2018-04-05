#pragma once
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
#include "Tempo.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
namespace QualitativeClustering {

	class Solution {
	public:
		double objective;
		int totalConstraints;
		int totalVariables;
		double timeSolve;
		std::vector<int> groups;
		std::vector<std::vector<int>> solutionMatrix;
		Solution() {}
	};

	class ILP {
	public:
		std::string problem;
		int n, m;
		std::vector<std::vector<int>>S;

		ILP(std::string p, int threshold = 0) {
			problem = p;

			std::ifstream finput;
			finput.open(problem.c_str(), std::fstream::in);
			if (finput.is_open() != true) {
				std::cerr << "The file " << problem << " does not exist" << std::endl;
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
		}

		Solution execute(int modelType) {
			Solution s;
			IloEnv   env;
			try {
				IloModel model(env);

				IloArray<IloNumVarArray> x(env, n);
				for (IloInt i = 0; i < n; i++)
					x[i] = IloNumVarArray(env, n, 0, 1, ILOBOOL);

				IloExpr obj(env);
				for (int i = 0; i < n; i++) {
					for (int j = i + 1; j < n; j++) {
						string name = "x" + std::to_string(i) + "." + std::to_string(j);
						x[i][j].setName(name.c_str());
						obj += S[i][j] * x[i][j];
					}
				}
				model.add(IloMaximize(env, obj));

				IloRangeArray con(env);

				for (int i = 0; i < n; i++) {
					for (int j = i + 1; j < n; j++) {
						for (int k = j + 1; k < n; k++) {

							if (modelType == 0) {
								con.add(x[i][j] + x[j][k] - x[i][k] <= 1);
								con.add(x[i][j] - x[j][k] + x[i][k] <= 1);
								con.add(-x[i][j] + x[j][k] + x[i][k] <= 1);
							}

							if (modelType == 1) {
								if (S[i][j] >= 0 || S[j][k] >= 0) {
									con.add(x[i][j] + x[j][k] - x[i][k] <= 1);
								}
								if (S[i][j] >= 0 || S[i][k] >= 0) {
									con.add(x[i][j] - x[j][k] + x[i][k] <= 1);
								}
								if (S[j][k] >= 0 || S[i][k] >= 0) {
									con.add(-x[i][j] + x[j][k] + x[i][k] <= 1);
								}
							}

							if (modelType == 2) {
								if (S[i][j] + S[j][k] >= 0) {
									con.add(x[i][j] + x[j][k] - x[i][k] <= 1);
								}
								if (S[i][j] + S[i][k] >= 0) {
									con.add(x[i][j] - x[j][k] + x[i][k] <= 1);
								}
								if (S[j][k] + S[i][k] >= 0) {
									con.add(-x[i][j] + x[j][k] + x[i][k] <= 1);
								}
							}

							if (modelType == 3) {
								if ((S[i][j] + S[j][k] >= 0) && S[i][j] >= 0 && S[i][k] <= 0) {
									con.add(x[i][j] + x[j][k] - x[i][k] <= 1);
								}
								if ((S[i][j] + S[i][k] >= 0) && S[i][j] >= 0 && S[j][k] <= 0) {
									con.add(x[i][j] - x[j][k] + x[i][k] <= 1);
								}
								if ((S[i][k] + S[j][k] >= 0) && S[i][k] >= 0 && S[i][j] <= 0) {
									con.add(-x[i][j] + x[j][k] + x[i][k] <= 1);
								}
							}
						}
					}
				}
				model.add(con);

				IloCplex cplex(model);
				cplex.setOut(env.getNullStream());

				double timeSolve = get_wall_time();

				if (!cplex.solve()) {
					env.error() << "Failed to optimize LP" << std::endl;
					throw(-1);
				}

				timeSolve = get_wall_time() - timeSolve;

				s.objective = cplex.getObjValue();
				s.totalConstraints = cplex.getNrows();
				s.totalVariables = cplex.getNcols();
				s.timeSolve = timeSolve;

				s.solutionMatrix.resize(n);
				s.groups.resize(n, -1);
				int	groupID = 0;

				for (int i = 0; i < n; i++) {
					s.solutionMatrix[i].resize(n, 0);

					for (int j = i + 1; j < n; j++) {

						try {
							s.solutionMatrix[i][j] = abs(round(cplex.getValue(x[i][j])));
						}
						catch (IloException& e) {}

						if (s.solutionMatrix[i][j] > 0) {
							if (s.groups[i] == -1 && s.groups[j] == -1) {
								s.groups[i] = groupID;
								s.groups[j] = groupID;
								groupID = groupID + 1;
							}
							else {
								if (s.groups[i] == -1)
									s.groups[i] = s.groups[j];
								else
									s.groups[j] = s.groups[i];
							}
						}
					}
				}

				// Objects that remained alone create its own group
				for (int i = 0; i < n; i++) {
					if (s.groups[i] == -1)
						s.groups[i] = groupID++;
				}

			}
			catch (IloException& e) {
				std::cerr << "Concert exception caught: " << e << endl;
			}
			catch (...) {
				std::cerr << "Unknown exception caught" << endl;
			}

			env.end();

			return s;
		}
	};
}

