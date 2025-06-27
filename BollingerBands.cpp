#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

double calculateSMA(const vector<double>& prices, int start, int period) {
    double sum = 0.0;
    for (int i = start; i < start + period; ++i) {
        sum += prices[i];
    }
    return sum / period;
}

double calculateStdDev(const vector<double>& prices, int start, int period, double mean) {
    double variance = 0.0;
    for (int i = start; i < start + period; ++i) {
        variance += (prices[i] - mean) * (prices[i] - mean);
    }
    return sqrt(variance / period);
}

int main() {
    vector<double> prices;
    int numPrices, period;
    double multiplier;

    cout << "Enter number of prices: ";
    cin >> numPrices;

    if (numPrices <= 0) {
        cout << "Invalid number of prices.\n";
        return 1;
    }

    cout << "Enter the prices:\n";
    for (int i = 0; i < numPrices; ++i) {
        double price;
        cin >> price;
        prices.push_back(price);
    }

    cout << "Enter the Bollinger Band period: ";
    cin >> period;

    if (period <= 0 || period > numPrices) {
        cout << "Invalid period.\n";
        return 1;
    }

    cout << "Enter the multiplier for standard deviation: ";
    cin >> multiplier;

    cout << "\nIndex\tPrice\tSMA\t\tUpper\t\tLower\t\tAction\n";

    for (int i = 0; i <= numPrices - period; ++i) {
        double sma = calculateSMA(prices, i, period);
        double stdDev = calculateStdDev(prices, i, period, sma);
        double upper = sma + multiplier * stdDev;
        double lower = sma - multiplier * stdDev;
        double currentPrice = prices[i + period - 1];

        string action = "Hold";
        if (currentPrice > upper) {
            action = "Sell";
        }
        else if (currentPrice < lower) {
            action = "Buy";
        }

        cout << (i + period - 1) << "\t"
            << currentPrice << "\t"
            << sma << "\t"
            << upper << "\t"
            << lower << "\t"
            << action << endl;
    }

    return 0;
}