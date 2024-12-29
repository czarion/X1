#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

// City structure to store x, y coordinates
struct City {
    double x, y;
};

// Calculate Euclidean distance between two cities
double euclideanDistance(const City &a, const City &b) {
    return sqrt((a.x - b.x) * (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Calculate total cost of a TSP tour
double calculateTotalDistance(const vector<City> &cities, const vector<int> &tour) {
    double distance = 0.0;
    for (size_t i = 0; i < tour.size(); i++) {
        int cityA = tour[i];
        int cityB = tour[(i + 1) % tour.size()]; // Wrap around to the start
        distance += euclideanDistance(cities[cityA], cities[cityB]);
    }
    return distance;
}

// Function to parse TSP data file
vector<City> loadCities(const string &filename) {
    vector<City> cities;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int id;
        City city;
        ss >> id >> city.x >> city.y;
        cities.push_back(city);
    }
    return cities;
}

// Simulated Annealing algorithm
vector<int> simulatedAnnealing(vector<City> &cities, int maxIterations = 10000, double initialTemp = 1000.0, double coolingRate = 0.995) {
    int n = cities.size();
    vector<int> currentTour(n);
    for (int i = 0; i < n; i++) currentTour[i] = i; // Initial tour
    random_shuffle(currentTour.begin(), currentTour.end()); // Randomize initial solution
    vector<int> bestTour = currentTour;
    double currentCost = calculateTotalDistance(cities, currentTour);
    double bestCost = currentCost;

    double temp = initialTemp;

    for (int iter = 0; iter < maxIterations; iter++) {
        int city1 = rand() % n;
        int city2 = rand() % n;
        swap(currentTour[city1], currentTour[city2]); // Generate a neighbor by swapping two cities

        double newCost = calculateTotalDistance(cities, currentTour);
        double delta = newCost - currentCost;

        if (delta < 0 || (exp(-delta / temp) > (double)rand() / RAND_MAX)) {
            // Accept the new solution
            currentCost = newCost;
            if (currentCost < bestCost) {
                bestTour = currentTour;
                bestCost = currentCost;
            }
        } else {
            // Revert the swap
            swap(currentTour[city1], currentTour[city2]);
        }

        // Cool down
        temp *= coolingRate;
    }

    cout << "Final Cost: " << bestCost << endl;
    return bestTour;
}

// Main function
int main() {
    srand(time(NULL));

    string filename;

    cout << "Enter the TSPLIB file name: ";
    cin >> filename;

    vector<City> cities = loadCities(filename);

    vector<int> optimalTour = simulatedAnnealing(cities);

    // Output the result
    cout << "Optimal Tour: ";
    for (int city : optimalTour) {
        cout << city << " ";
    }
    cout << endl;

    return 0;
}