#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iomanip>
#include <fstream>

/**
 * @author Alexander Iliashenko
 * @version 0.1
 * Procedural realization of Ant colony optimization(ACO) algorithm
 * that resolve simple example of Travelling salesman problem(TSP).
 */

#define deltaOfTime 1000
#define numberOfAnts 5
#define numberOfCities 20


using namespace std;

int alpha = 2;                          // controls the pheromone importance
int bbeta = 4;                          // controls the distance priority
double randomFactor = 0.01;           // pheromone decrease factor
double Q = 2.0;                      // total amount of pheromone left on the trail by each Ant

/**
 * Creating graph, where vertex if city.
 * @param dist
 */
void graph(int dist[numberOfCities][numberOfCities])
{
    for (int i = 0; i < numberOfCities; i++) {
        for (int j = i; j < numberOfCities; j++) {
            if (j == i) {
                dist[i][j] = 0;
            } else {
                int d = rand() % 8 + 1;
                dist[i][j] = d;
                dist[j][i] = d;
            }
        }
    }
}

/**
 * Syso of ants.
 * @param ants
 */
void antsOutput(int ants[][numberOfCities])
{
    for (int i = 0; i < numberOfAnts; i++) {
        for (int j = 0; j < numberOfCities; j++) {
            cout << ants[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/**
 * Syso adjacency matrix for our graph.
 * @param dist
 */
void distOutput(int dist[][numberOfCities])
{
    cout << "\n\n\t\t---adjacency matrix---\n\n" << endl;
    for (int k = 0; k < numberOfCities; k++) {
        cout << right << setw( 3 ) << k << " ";
    }
    std::cout << "\n" << std::string(4*numberOfCities, '-') << "\n";
    for (int i = 0; i < numberOfCities; i++) {
        for (int j = 0; j < numberOfCities; j++) {
            cout << right << setw( 3 ) << dist[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/**
 * Syso pheromones array.
 * @param pherom1s
 */
void pherom1sOutput(double pherom1s[][numberOfCities])
{
    for (int i = 0; i < numberOfCities; i++) {
        for (int j = 0; j < numberOfCities; j++) {
            cout << pherom1s[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/**
 * Input to all pheromones default values.
 * @param pherom1s
 */
void initPherom1s(double pherom1s[][numberOfCities])
{
    for (int i = 0; i < numberOfCities; i++) {
        for (int j = 0; j < numberOfCities; j++) {
            pherom1s[i][j] = 0.01;
        }
    }
}

int indexOfTarget(int target, int trail[]) {
    int i;
    for (i = 0; i < numberOfCities; i++) {
        if (trail[i] == target) return i;
    }
    cout << "target not found in indexOfTarget:   " << i << endl;
}

void initAnts(int ants[numberOfAnts][numberOfCities])
{
    for (int i = 0; i < numberOfAnts; i++) {
        int start = rand() % numberOfCities;
        int trail[numberOfCities];
        for (int j = 0; j < numberOfCities; j++) {
            trail[j] = j;
        }
        random_shuffle(trail, trail + numberOfCities);
        int index = indexOfTarget(start, trail);
        int tmp = trail[index];
        trail[0] = trail[index];
        trail[index] = tmp;
        //swap(trail[0], trail[index]);
        for (int j = 0; j < numberOfCities; j++) {
            ants[i][j] = trail[j];
        }
    }
}

/**
 * Calculating distance for trail.
 * @param x
 * @param dist
 * @return
 */
double lengthCalc(int x[], int dist[][numberOfCities])
{
    double ans = 0.0;
    for (int i = 0; i < numberOfCities - 1; i++) {
        ans += dist[x[i]][x[i + 1]];
    }
    return ans;
}

/**
 * Syso trail and length format.
 * @param ants
 * @param dist
 */
void lengthOutput(int ants[][numberOfCities], int dist[][numberOfCities])
{
    for (int i = 0; i < numberOfAnts; i++) {
        cout << i << ": [ ";
        for (int j = 0; j < 4; j++) {
            cout << right <<setw(4) << ants[i][j] << " ";
        }
        cout << " ------- ";
        for (int j = numberOfCities - 4; j < numberOfCities; j++) {
            cout << right <<setw(4) << ants[i][j] << " ";
        }
        cout << "] len = ";
        double len = lengthCalc(ants[i], dist);
        cout << len << endl;
    }
}

/**
 * Finding best trail by distance length value.
 * @param ants
 * @param dist
 * @param bestTrail
 */
void bestTrail(int ants[][numberOfCities], int dist[][numberOfCities], int bestTrail[])
{
    double bestlength = lengthCalc(ants[0], dist);
    int index = 0;
    for (int i = 1; i < numberOfAnts; i++) {
        double len = lengthCalc(ants[i], dist);
        if (len < bestlength) {
            bestlength = len;
            index = i;
        }
    }

    for (int i = 0; i < numberOfCities; i++) {
        bestTrail[i] = ants[index][i];
    }
}

/**
 * Finding probabilty by formula.
 * @param k
 * @param cityX
 * @param visited
 * @param pherom1s
 * @param dist
 * @param probs
 */
void prob(int k, int cityX, bool visited[], double pherom1s[][numberOfCities], int dist[][numberOfCities], double probs[])
{
    double tau[numberOfCities];
    double sum = 0.0;
    for (int i = 0; i < numberOfCities; i++) {
        if (i == cityX) {
            tau[i] = 0.0;
        } else if (visited[i] == true) {
            tau[i] = 0.0;
        } else {
            tau[i] = (pow(pherom1s[cityX][i], alpha*1.0)) * (pow((1.0 / dist[cityX][i]*1.0), bbeta*1.0));
            if (tau[i] < 0.0001) {
                tau[i] = 0.0001;
            } else if (tau[i] > DBL_MAX / numberOfCities * 100) {
                tau[i] = DBL_MAX / numberOfCities * 100;
            }
        }
        sum += tau[i];
    }
    for (int i = 0; i < numberOfCities; i++) {
        probs[i] = tau[i] / sum;
    }
}

int nextCity(int k, int cityX, bool visited[], double pherom1s[][numberOfCities], int dist[][numberOfCities])
{
    double probs[numberOfCities];
    prob(k, cityX, visited, pherom1s, dist, probs);
    double cum[numberOfCities + 1];
    cum[0] = 0.0;
    for (int i = 0; i < numberOfCities; i++) {
        cum[i + 1] = cum[i] + probs[i];
    }
    double p = (double)rand() / (double)RAND_MAX;
    for (int i = 0; i < numberOfCities; i++) {
        if (p >= cum[i] && p < cum[i + 1]) {
            return i;
        }
    }
    cout << "failure\n";
}

/**
 * Creating trail for ants.
 * @param k
 * @param start
 * @param pherom1s
 * @param dist
 * @param newTrail
 */
void createTrail(int k, int start, double pherom1s[][numberOfCities], int dist[][numberOfCities], int newTrail[])
{
    int trail[numberOfCities];
    bool visited[numberOfCities];
    for (int i = 0; i < numberOfCities; i++) {
        visited[i] = false;
    }
    trail[0] = start;
    visited[start] = true;
    for (int i = 0; i < numberOfCities - 1; i++) {
        int cityX = trail[i];
        int next = nextCity(k, cityX, visited, pherom1s, dist);
        trail[i + 1] = next;
        visited[next] = true;
    }
    for (int i = 0; i < numberOfCities; i++) {
        newTrail[i] = trail[i];
    }
}

void updateAnts(int ants[][numberOfCities], double pherom1s[][numberOfCities], int dist[][numberOfCities])
{
    for (int i = 0; i < numberOfAnts; i++) {
        int start = rand() % numberOfCities;
        int newTrail[numberOfCities];
        createTrail(i, start, pherom1s, dist, newTrail);
        for (int j = 0; j < numberOfCities; j++) {
            ants[i][j] = newTrail[j];
        }
    }
}

bool edgePresentinTrail(int cityX, int cityY, int trail[])
{
    int lastIndex = numberOfCities - 1;
    int index = indexOfTarget(cityX, trail);
    if (index == 0 && trail[1] == cityY) return true;
    else if (index == 0 && trail[lastIndex] == cityY) return true;
    else if (index == 0) return false;
    else if (index == lastIndex && trail[lastIndex - 1] == cityY) return true;
    else if (index == lastIndex && trail[0] == cityY) return true;
    else if (index == lastIndex) return false;
    else if (trail[index - 1] == cityY) return true;
    else if (trail[index + 1] == cityY) return true;
    else return false;
}

/**
 * Updating pheromones by formula.
 * @param ants
 * @param pherom1s
 * @param dist
 */
void updatePherom1s(int ants[][numberOfCities], double pherom1s[][numberOfCities], int dist[][numberOfCities])
{
    for (int i = 0; i < numberOfCities; i++) {
        for (int j = i + 1; j < numberOfCities; j++) {
            for (int k = 0; k < numberOfAnts; k++) {
                double length = lengthCalc(ants[k], dist);
                double decrease = (1.0 - randomFactor) * pherom1s[i][j];
                double increase = 0.0;
                if (edgePresentinTrail(i, j, ants[k]) == true) {
                    increase = Q / length;
                }
                pherom1s[i][j] = increase + decrease;
                if (pherom1s[i][j] < 0.0001) {
                    pherom1s[i][j] = 0.0001;
                } else if (pherom1s[i][j] > 100000.0) {
                    pherom1s[i][j] = 100000.0;
                }
                pherom1s[j][i] = pherom1s[i][j];
            }
        }
    }
}

void output(int besttrail[])
{
    for (int i = 0; i < numberOfCities; i++) {
        cout << besttrail[i] << " ";
        if (i > 0 && i % 20 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

int main()
{
    cout << "\n\t\t---ant colony info---\n\n";
    cout << "number of cities: " << numberOfCities << endl;
    cout << "number of ants: " << numberOfAnts << endl;
    cout << "max time: " << deltaOfTime << endl;
    cout << "alpha coef (pheromone influence): " << alpha << endl;
    cout << "bbeta coef (local node influence): " << bbeta << endl;
    cout << "random factor coef (pheromone evaporation coefficient): " << randomFactor << endl;
    cout << "Q (pheromone deposit factor): " << Q << endl;
    cout << "\n\t\t---undirected graph with random edge lengths---\n";

    int dist[numberOfCities][numberOfCities];
    graph(dist);                             // creating graph
    ofstream f;
    f.open("Output.txt");
    if (f.good()) {
        cout << "\n write adj matrix to file \n";
    }
    f << right << setw( 10 ) << "0 ";
    for (int k = 1; k < numberOfCities; k++) {
        f << right << setw( 3 ) << k << " ";
    }
    f << "\n" << std::string(4.5*numberOfCities, '-') << "\n";
    for (int i = 0; i < numberOfCities; i++) {
        for (int j = 0; j < numberOfCities; j++) {
          if (j==0){ f << right << setw( 3 ) << i  << " | " << right << setw( 3 ) << dist[i][j] << " ";}else{
             f << right << setw( 3 ) << dist[i][j] << " ";
        }}
        f << endl;
    }

    cout << "\ninit ants to random trails:\n";  // init trails for ants
    int ants[numberOfAnts][numberOfCities];
    initAnts(ants);
    lengthOutput(ants, dist);

    int bestTrailArray[numberOfCities];
    bestTrail(ants, dist, bestTrailArray);

    double bestlength = lengthCalc(bestTrailArray, dist);
    bestlength += dist[bestTrailArray[0]][bestTrailArray[numberOfCities - 1]];

    cout << "\nbest initial trail length: " << bestlength << endl;

    cout << "\n\t\t---init pherom1s on trails---\n\n";
    double pherom1s[numberOfCities][numberOfCities];      // init pherom1ns
    initPherom1s(pherom1s);
    int time = 0;
    while (time < deltaOfTime) {
        updateAnts(ants, pherom1s, dist);
        updatePherom1s(ants, pherom1s, dist);
        int currBestTrail[numberOfCities];
        bestTrail(ants, dist, currBestTrail);
        double currbestlength = lengthCalc(currBestTrail, dist);
        currbestlength += dist[currBestTrail[0]][currBestTrail[numberOfCities - 1]];

        if (currbestlength < bestlength) {
            bestlength = currbestlength;
            for (int i = 0; i < numberOfCities; i++) {
                bestTrailArray[i] = currBestTrail[i];
            }
            cout << "New Best Trail of: " << bestlength << " found at time : " << time << endl;
        }
        ++time;
    }

    cout << "\n\t\t---best trail found---\n";

    output(bestTrailArray);

    cout << "\nlength of best trail found: " << bestlength << endl;

    return 0;
}



