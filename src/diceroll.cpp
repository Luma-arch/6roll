#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <thread>
#include <vector>
#include <atomic>

void roll_dice(int numRoll, std::mt19937 gen, std::uniform_int_distribution<>& distr, std::atomic<int>& uno, std::atomic<int>& dos, std::atomic<int>& tres, std::atomic<int>& cuatro,
    std::atomic<int>& cinco, std::atomic<int>& seis) {
    int local_uno = 0;
    int local_dos = 0;
    int local_tres = 0;
    int local_cuatro = 0;
    int local_cinco = 0;
    int local_seis = 0;

    for (int i = 0; i < numRoll; ++i) {
        int roll = distr(gen); // Generate a random number between 1 and 6
        if (roll == 1) {
            ++local_uno;
        }
        else if (roll == 2) {
            ++local_dos;
        }
        else if (roll == 3) {
            ++local_tres;
        }
        else if (roll == 4) {
            ++local_cuatro;
        }
        else if (roll == 5) {
            ++local_cinco;
        }
        else if (roll == 6) {
            ++local_seis;
        }
    }

    uno += local_uno;
    dos += local_dos;
    tres += local_tres;
    cuatro += local_cuatro;
    cinco += local_cinco;
    seis += local_seis;
}

int main() {
    int numRoll = 0;
    int numThreads = 3;

    std::cout << "Enter the number of rolls you want: ";
    std::cin >> numRoll;

    std::random_device rd;

    std::uniform_int_distribution<> distr(1, 6);

    std::chrono::steady_clock sc;

    std::atomic<int> uno(0);
    std::atomic<int> dos(0);
    std::atomic<int> tres(0);
    std::atomic<int> cuatro(0);
    std::atomic<int> cinco(0);
    std::atomic<int> seis(0);

    auto start = sc.now();

    std::vector<std::thread> threads;
    int rollsPerThread = numRoll / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        std::mt19937 gen(rd() + i);  // Create a local generator for each thread
        threads.emplace_back(roll_dice, rollsPerThread, gen, std::ref(distr), std::ref(uno), std::ref(dos), std::ref(tres), std::ref(cuatro), std::ref(cinco), std::ref(seis));
    }

    for (auto& t : threads) {
        t.join();
    }

    int remainingRolls = numRoll % numThreads;
    std::mt19937 gen(rd() + numThreads);  // Create a generator for the remaining numbers
    roll_dice(remainingRolls, gen, distr, uno, dos, tres, cuatro, cinco, seis);

    double probability = static_cast<double>(seis) / static_cast<double>(numRoll);

    auto end = sc.now();
    auto time_span = static_cast<std::chrono::duration<double>>(end - start);
    std::cout << std::setprecision(15) << std::fixed << "Time taken: " << time_span.count() << " seconds " << std::endl;
    std::cout << std::endl;

    std::cout << "1 rolled: " << uno << std::endl;
    std::cout << "2 rolled: " << dos << std::endl;
    std::cout << "3 rolled: " << tres << std::endl;
    std::cout << "4 rolled: " << cuatro << std::endl;
    std::cout << "5 rolled: " << cinco << std::endl;
    std::cout << "6 rolled: " << seis << std::endl;
    std::cout << "Total rolls: " << numRoll << std::endl;
    std::cout << std::endl;

    std::cout << std::setprecision(15) << std::fixed << "Experimental probability of rolling a 6: " << probability * 100 << "%" << std::endl;
    std::cout << std::setprecision(15) << std::fixed << "Theoretical probability of rolling a 6: " << (1.0 / 6.0) * 100 << "%" << std::endl;
    std::cout << std::endl;

    system("pause");

    return 0;
}
