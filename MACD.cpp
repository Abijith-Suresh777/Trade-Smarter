#include <iostream>
#include <vector>

using namespace std;

vector<double> calculateEMA(const vector<double>& prices, int period, int startIndex = 0) {
    vector<double> ema;

    if (prices.size() < period + startIndex) return ema;

    double k = 2.0 / (period + 1);
    double sum = 0.0;

    for (int i = startIndex; i < startIndex + period; ++i) {
        sum += prices[i];
    }

    double prevEMA = sum / period;
    ema.push_back(prevEMA);

    for (size_t i = startIndex + period; i < prices.size(); ++i) {
        double currentEMA = prices[i] * k + prevEMA * (1 - k);
        ema.push_back(currentEMA);
        prevEMA = currentEMA;
    }

    return ema;
}

vector<double> calculateMACD(const vector<double>& fastEMA, const vector<double>& slowEMA) {
    vector<double> macd;
    int offset = fastEMA.size() - slowEMA.size();
    if (offset < 0) return macd;

    for (size_t i = 0; i < slowEMA.size(); ++i) {
        macd.push_back(fastEMA[i + offset] - slowEMA[i]);
    }

    return macd;
}

vector<double> calculateHistogram(const vector<double>& macdLine, const vector<double>& signalLine) {
    vector<double> histogram;
    int offset = macdLine.size() - signalLine.size();
    if (offset < 0) return histogram;

    for (size_t i = 0; i < signalLine.size(); ++i) {
        histogram.push_back(macdLine[i + offset] - signalLine[i]);
    }

    return histogram;
}

vector<string> generateSignals(const vector<double>& macdLine, const vector<double>& signalLine) {
    vector<string> signals;
    int offset = macdLine.size() - signalLine.size();
    if (offset < 1) return signals;

    signals.push_back("Hold");

    for (size_t i = 1; i < signalLine.size(); ++i) {
        double prevMACD = macdLine[offset + i - 1];
        double prevSignal = signalLine[i - 1];
        double currMACD = macdLine[offset + i];
        double currSignal = signalLine[i];

        if (prevMACD < prevSignal && currMACD > currSignal) {
            signals.push_back("Buy");
        }
        else if (prevMACD > prevSignal && currMACD < currSignal) {
            signals.push_back("Sell");
        }
        else {
            signals.push_back("Hold");
        }
    }

    return signals;
}

int main() {
    vector<double> prices;
    int numPrices;

    cout << "Enter the number of prices: ";
    cin >> numPrices;

    if (numPrices <= 0) {
        cout << "Number of prices must be positive.\n";
        return 1;
    }

    cout << "Enter the prices:\n";
    for (int i = 0; i < numPrices; ++i) {
        double price;
        cin >> price;
        prices.push_back(price);
    }

    int fastPeriod, slowPeriod, signalPeriod;
    cout << "Enter the fast EMA period: ";
    cin >> fastPeriod;
    cout << "Enter the slow EMA period: ";
    cin >> slowPeriod;
    cout << "Enter the signal line period: ";
    cin >> signalPeriod;

    if (fastPeriod <= 0 || slowPeriod <= 0 || signalPeriod <= 0 || fastPeriod >= slowPeriod) {
        cout << "Invalid periods. Ensure they are positive and fast < slow.\n";
        return 1;
    }

    vector<double> fastEMA = calculateEMA(prices, fastPeriod);
    vector<double> slowEMA = calculateEMA(prices, slowPeriod);
    vector<double> macdLine = calculateMACD(fastEMA, slowEMA);
    vector<double> signalLine = calculateEMA(macdLine, signalPeriod);
    vector<double> histogram = calculateHistogram(macdLine, signalLine);
    vector<string> signals = generateSignals(macdLine, signalLine);

    if (signals.empty()) {
        cout << "Not enough data to compute signals.\n";
        return 1;
    }

    cout << "\nIndex\tPrice\tMACD\t\tSignal\t\tHistogram\tAction\n";
    size_t histogramOffset = prices.size() - histogram.size();

    for (size_t i = 0; i < histogram.size(); ++i) {
        size_t idx = histogramOffset + i;
        cout << idx << "\t"
            << prices[idx] << "\t"
            << macdLine[i + macdLine.size() - histogram.size()] << "\t"
            << signalLine[i] << "\t"
            << histogram[i] << "\t"
            << signals[i] << endl;
    }

    return 0;
}