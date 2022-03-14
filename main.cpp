#include <iostream>
#include <vector>

using namespace std;

template <typename T>
void printBits(T x, int size){
    T y = 0;
    for(int i = 0; i < size; i++){
        y = (y << 1) ^ (x % 2);
        x = x >> 1;
    }
    for (int i = 0; i < size; i++){
        cout<<y%2;
        y= y>>1;
    }
    cout << endl;
}
template <typename T>
T getBit(T x, int index){
    return(x >> index) % 2;
}

template <typename T>
T setBit(T x, int index, int s){
    if (s==0){
        return x&~(1 << index);
    }
    else {
        return x|(1 << index);
    }

}

template <typename T>
T leftRotate(T x, int m, int size){
    T firstM = (x >> (size-m));
    return (x<<m) | firstM;
}


void lfsr(unsigned int path, unsigned int initStates, unsigned int randomBits){
    if (randomBits < 1) return;

    //Compute indexes to be XORed
    int feedback[32];
    int feedbackIndexes = 0;
    for(int i = 0; i < 32; i++){
        if(path % 2 == 1){
            feedback[feedbackIndexes] = i;
            feedbackIndexes++;

        }
        path = path >> 1;
    }


    unsigned int lfsr = initStates;

    //Main loop
    for(int i = 0; i < randomBits; i++){
        //print output bit
        cout<< lfsr %2;

        //Compute nextBit
        unsigned int nextBit = 0;
        //XOR bit at each of the feedback indexes with 1
        for (int n = 0; n < feedbackIndexes; n++)
            nextBit = nextBit ^ getBit(lfsr, feedback[n]);

        lfsr = (lfsr >> 1) | (nextBit << 31);

    }
    cout << endl;
}

unsigned long long expansion(unsigned int input){
    unsigned long long output = 0;
    int expand[48] = { 1, 32, 31, 30, 29, 28, 29, 28,
                       27, 26, 25, 24, 25, 24, 23, 22,
                       21, 20, 21, 20, 19, 18, 17, 16,
                       17, 16, 15, 14, 13, 12, 13, 12,
                       11, 10, 9, 8, 9, 8, 7, 6,
                       5, 4, 5, 4, 3, 2, 1, 32 };
    for (int i : expand){
        output = (output << 1) | getBit(input, i - 1);
    }

    return output;
}

unsigned char s1Box(unsigned char input){

    int sBox[4][16] = {{ 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
                       { 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
                       { 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
                       { 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 }};

    int row = (getBit(input, 5) << 1) | getBit(input, 0);

    int col = (getBit(input, 4) << 3) |
              (getBit(input, 3) << 2) |
              (getBit(input, 2) << 1) |
              (getBit(input,1));

    return sBox[row][col];
}

unsigned int permute(unsigned int input){
    unsigned int output = 0;
    int perm[32] = { 16, 7, 20, 21,
                     29, 12, 28, 17,
                     1, 15, 23, 26,
                     5, 18, 31, 10,
                     2, 8, 24, 14,
                     32, 27, 3, 9,
                     19, 13, 30, 6,
                     22, 11, 4, 25 };
    for (int i : perm){

        output = (output << 1) | getBit(input, 32-i);
    }
    return output;
}

void shiftRows(unsigned int &x0, unsigned int &x1, unsigned int &x2, unsigned &x3){

    unsigned int w0 = 4278190080; //Selects word 0
    unsigned int w1 = 16711680; //Selects word 1
    unsigned int w2 = 65280; //Selects word 2
    unsigned int w3 = 255; //Selects word 3
    unsigned int newX0 = (x0 & w0) | (x1 & w1) | (x2 & w2) | (x3 & w3);
    unsigned int newX1 = (x1 & w0) | (x2 & w1) | (x3 & w2) | (x0 & w3);
    unsigned int newX2 = (x2 & w0) | (x3 & w1) | (x0 & w2) | (x1 & w3);
    unsigned int newX3 = (x3 & w0) | (x0 & w1) | (x1 & w2) | (x2 & w3);
    x0 = newX0;
    x1 = newX1;
    x2 = newX2;
    x3 = newX3;



}

int gcd(int a, int b){

    if(a<0) a *= -1;
    if(b<0) b *= -1;
    if(a==0) return b;
    if(b==0) return a;
    if(a>b) return gcd(b, a %b);
    else return gcd(a, b%a);
}

int main() {
    cout<<"lfsr(3, 34, 50):" << endl;
    lfsr(3,34,50);
    cout<<"lfsr(3, 34, 50):" << endl;
    //lfsrM(3,34,50);

    cout << "printBits(expansion(1), 64):" << endl;
    printBits(expansion(1),64);
    cout<< "s1Box(37): " << (int)s1Box(37) << endl;
    cout<< "permute(1): " << permute(1) << endl;

    unsigned int x0 = 2000000000;
    unsigned int x1 = 2000000;
    unsigned int x2 = 2000;
    unsigned int x3 = 2;

    shiftRows(x0, x1, x2, x3);
    cout << "x0: "<< x0 <<endl;
    cout << "x1: "<< x1 <<endl;
    cout << "x2: "<< x2 <<endl;
    cout << "x3: "<< x3 <<endl;

    cout << "gcd(3, 10): " << gcd(3,10) << endl;
    cout << "gcd(21, 15): " << gcd(21,15) << endl;
    cout << "gcd(-16, 24): " << gcd(-16,24) << endl;

}