#include <iostream>
#include <vector>

using namespace std;

vector<double> calculateRSI(const vector<double>& prices, int period) {
    vector<double> rsi;

    if (prices.size() <= period) return rsi;

    double gainSum = 0.0, lossSum = 0.0;

    for (int i = 1; i <= period; ++i) {
        double change = prices[i] - prices[i - 1];
        if (change > 0)
            gainSum += change;
        else
            lossSum -= change;
    }

    double avgGain = gainSum / period;
    double avgLoss = lossSum / period;

    double rs = (avgLoss == 0) ? 100 : avgGain / avgLoss;
    rsi.push_back(100.0 - (100.0 / (1 + rs)));

    for (size_t i = period + 1; i < prices.size(); ++i) {
        double change = prices[i] - prices[i - 1];
        double gain = (change > 0) ? change : 0;
        double loss = (change < 0) ? -change : 0;

        avgGain = ((avgGain * (period - 1)) + gain) / period;
        avgLoss = ((avgLoss * (period - 1)) + loss) / period;

        rs = (avgLoss == 0) ? 100 : avgGain / avgLoss;
        rsi.push_back(100.0 - (100.0 / (1 + rs)));
    }

    return rsi;
}

int main() {
    vector<double> prices;
    int numPrices, period;

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

    cout << "Enter RSI period: ";
    cin >> period;

    if (period <= 0 || period >= prices.size()) {
        cout << "Invalid RSI period.\n";
        return 1;
    }

    vector<double> rsiValues = calculateRSI(prices, period);

    if (rsiValues.empty()) {
        cout << "Not enough data to calculate RSI.\n";
        return 1;
    }

    cout << "\nIndex\tPrice\tRSI\t\tSignal\n";
    size_t startIndex = prices.size() - rsiValues.size();

    for (size_t i = 0; i < rsiValues.size(); ++i) {
        double price = prices[startIndex + i];
        double rsi = rsiValues[i];
        string signal = "Hold";

        if (rsi < 30)
            signal = "Buy";
        else if (rsi > 70)
            signal = "Sell";

        cout << (startIndex + i) << "\t" << price << "\t" << rsi << "\t\t" << signal << endl;
    }

    return 0;
}