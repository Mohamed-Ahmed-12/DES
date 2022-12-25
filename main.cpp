/*
Implemented by Mohamed Ahmed and Ammar Yasser
we are students in Faculty of Computers and Artificial Intelligence - Cairo University

*/

#include <iostream>
#include <cmath>
#include <string>
#include <bitset>
#include <sstream>

using namespace std;

int num_of_zeros ;

string BinaryToASCII(string binaryString) {
    string text = "";
    stringstream sstream(binaryString);
    while (sstream.good())
    {
        bitset<8> bits;
        sstream >> bits;
        text += char(bits.to_ulong());
    }
    return text;
}

string DecimalToBinary(int dec) {
	if (dec < 1) return "0";

	string binStr = "";

	while (dec > 0)
	{
		binStr = binStr.insert(0, string(1, (char)((dec % 2) + 48)));

		dec /= 2;
	}

	return binStr;
}

string ASCIIToBinary(string str) {
	string bin = "";
	int strLength = str.length();

	for (int i = 0; i < strLength; ++i)
	{
		string cBin = DecimalToBinary(str[i]);
		int cBinLength = cBin.length();

		if (cBinLength < 8) {
			for (size_t i = 0; i < (8 - cBinLength); i++)
				cBin = cBin.insert(0, "0");
		}

		bin += cBin;
	}

	return bin;
}

int BinaryToDecimal(string binaryString)
{
    int value = 0;
    int indexCounter = 0;
    for (int i = binaryString.length() - 1; i >= 0; i--)
    {

        if (binaryString[i] == '1')
        {
            value += pow(2, indexCounter);
        }
        indexCounter++;
    }
    return value;
}
/**/

string permute(string m, int arr[], int n)
{
    string new_ = "";
    for (int i = 0; i < n; i++)
    {
        new_ += m[arr[i] - 1];//because first bit in m its index is 0
    }
    return new_;
}

string xor_(string a, string b)
{
    string result = "";
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] == b[i])
        {
            result += "0";
        }
        else
        {
            result += "1";
        }
    }
    return result;
}


/* Generate Sub keys */

// Permutation Choice 1 take 64 bit of key and return 56 bit (remove parity 8 bits)

    int PC1[] =
    {
        57, 49, 41, 33, 25, 17, 9,
        1,  58, 50, 42, 34, 26, 18,
        10, 2,  59, 51, 43, 35, 27,
        19, 11, 3,  60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7,  62, 54, 46, 38, 30, 22,
        14, 6,  61, 53, 45, 37, 29,
        21, 13, 5,  28, 20, 12, 4
    };

    int PC2[] =
{
    14, 17, 11, 24, 1,  5,  3,  28,
    15, 6,  21, 10, 23, 19, 12, 4,
    26, 8,  16, 7,  27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32
};

int num_shift[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };// for sub keys

string shift_left(string k, int shifts)
{
    string s = "";
    for (int i = 0; i < shifts; i++)
    {
        for (int j = 1; j < 28; j++)
        {
            s += k[j];
        }
        s += k[0];
        k = s;
        s = "";
    }
    return k;
}


string * generate_subkeys(string key)
{
    static string keys[17];
    string kbits = ASCIIToBinary(key);
    string key_after_pc1 = permute(kbits, PC1, 56);
    string arrOfC[17];
    string arrOfD[17];
    arrOfC[0] = key_after_pc1.substr(0, 28);
    arrOfD[0] = key_after_pc1.substr(28,28);

    keys[0] = key_after_pc1;
    int num2 = 0;
    for (int i = 1; i < 17; i++)
    {
        arrOfC[i] = shift_left(arrOfC[i - 1], num_shift[i - 1]);
        arrOfD[i] = shift_left(arrOfD[i - 1], num_shift[i - 1]);
        keys[i] = arrOfC[i] + arrOfD[i];
        keys[i] = permute(keys[i], PC2, 48);
    }
    return keys;
}

/* f function */

//expand 32bit of R to 48bit to make xor with rounded key
int expand_table[] =
{
    32, 1, 2, 3, 4, 5, 4, 5,
    6, 7, 8, 9, 8, 9,  10, 11,
    12, 13, 12, 13, 14, 15, 16,17,
    16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25,24, 25, 26, 27,
    28, 29, 28, 29, 30, 31, 32, 1
};

//selection table
int s[8][4][16] = { {
        14, 4,  13, 1,  2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0,  7,
        0,  15, 7,  4,  14, 2,  13, 1,  10, 6,  12, 11, 9,  5,  3,  8,
        4,  1,  14, 8,  13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5,  0,
        15, 12, 8,  2,  4,  9,  1,  7,  5,  11, 3,  14, 10, 0,  6,  13
    }, {
        15, 1,  8,  14, 6,  11, 3,  4,  9,  7,  2,  13, 12, 0,  5,  10,
        3,  13, 4,  7,  15, 2,  8,  14, 12, 0,  1,  10, 6,  9,  11, 5,
        0,  14, 7,  11, 10, 4,  13, 1,  5,  8,  12, 6,  9,  3,  2,  15,
        13, 8,  10, 1,  3,  15, 4,  2,  11, 6,  7,  12, 0,  5,  14, 9
    }, {
        10, 0,  9,  14, 6,  3,  15, 5,  1,  13, 12, 7,  11, 4,  2,  8,
        13, 7,  0,  9,  3,  4,  6,  10, 2,  8,  5,  14, 12, 11, 15, 1,
        13, 6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12, 5,  10, 14, 7,
        1,  10, 13, 0,  6,  9,  8,  7,  4,  15, 14, 3,  11, 5,  2,  12
    }, {
        7,  13, 14, 3,  0,  6,  9,  10, 1,  2,  8,  5,  11, 12, 4,  15,
        13, 8,  11, 5,  6,  15, 0,  3,  4,  7,  2,  12, 1,  10, 14, 9,
        10, 6,  9,  0,  12, 11, 7,  13, 15, 1,  3,  14, 5,  2,  8,  4,
        3,  15, 0,  6,  10, 1,  13, 8,  9,  4,  5,  11, 12, 7,  2,  14
    }, {
        2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0,  14, 9,
        14, 11, 2,  12, 4,  7,  13, 1,  5,  0,  15, 10, 3,  9,  8,  6,
        4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3,  0,  14,
        11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4,  5,  3
    }, {
        12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3,  4,  14, 7,  5,  11,
        10, 15, 4,  2,  7,  12, 9,  5,  6,  1,  13, 14, 0,  11, 3,  8,
        9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1,  13, 11, 6,
        4,  3,  2,  12, 9,  5,  15, 10, 11, 14, 1,  7,  6,  0,  8,  13
    }, {
        4,  11, 2,  14, 15, 0,  8,  13, 3,  12, 9,  7,  5,  10, 6,  1,
        13, 0,  11, 7,  4,  9,  1,  10, 14, 3,  5,  12, 2,  15, 8,  6,
        1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0,  5,  9,  2,
        6,  11, 13, 8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2,  3,  12
    }, {
        13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7,
        1,  15, 13, 8,  10, 3,  7,  4,  12, 5,  6,  11, 0,  14, 9,  2,
        7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15, 3,  5,  8,
        2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5,  6,  11
    }
};


// each block of 4 original bits has been expanded to a block of 6 output bits
string expand_r(string r)
{
    return permute(r, expand_table, 48);
}


// use per table to permute the output of s-boxes
int per[] =
{
    16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23,
    26, 5, 18, 31, 10, 2,  8,  24, 14, 32, 27,
    3,  9, 19, 13, 30, 6,  22, 11, 4,  25
};

string f(string r, string k)  //r n-1 , k n
{
    string xor_result = xor_(expand_r(r), k);

    string sbox[8];

    for(int i = 0; i<8; i++)
    {
        sbox[i]= xor_result.substr(i*6,6);
    }

    // S-boxes

    int row[8];//hold 2 bit first and last
    int col[8];// hold 4 bit

    for (int i = 0; i < 8; i++)
    {
        col[i] = BinaryToDecimal(sbox[i].substr(1, 4));
        row[i] = BinaryToDecimal(sbox[i].substr(0, 1) + sbox[i].substr(4,5));

    }
    string val2;
    string val = "";
    for (int i = 0; i < 8 ; i++)
    {
        val2 = DecimalToBinary( s[i][row[i]][col[i]]);
        if (val2.length() == 1)
            val2 = "000"+val2;
        if (val2.length() == 2)
            val2 = "00"+val2;
        if (val2.length() == 3)
            val2 = "0" + val2;
        val += val2;

    }
    val = permute(val, per, 32);

    return  val;
}

/* DES Encryption and Decryption */

    //Initial Permutation Table
    int IP[] =
    {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9,  1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };

    //Final Permutation Table
    int FP[] =
    {
        40, 8,  48, 16, 56, 24, 64, 32, 39, 7,  47,
        15, 55, 23, 63, 31, 38, 6,  46, 14, 54, 22,
        62, 30, 37, 5,  45, 13, 53, 21, 61, 29, 36,
        4,  44, 12, 52, 20, 60, 28, 35, 3,  43, 11,
        51, 19, 59, 27, 34, 2,  42, 10, 50, 18, 58,
        26, 33, 1,  41, 9,  49, 17, 57, 25
    };

string handle_block_of_64(string message){
    string new_message = "";
    string message_bit = ASCIIToBinary(message);
    int j = 0;
    if (message_bit.length() % 64 == 0)
    {
        new_message = message_bit;
        //here message if equal 64 or 128 or 192 .... bits
    }
     else if (message_bit.length() > 64)
    {
        if (message_bit.length() % 64 != 0)
        {
            for (int i = 0; i < (64 - message_bit.length() % 64); i++)
            {
                new_message += "0";
                j = new_message.length();
            }
            new_message = new_message + message_bit ;
        }

    }
    else if (message_bit.length() < 64)
    {
        for (int i = 0; i < (64 - message_bit.length()); i++)
        {
            new_message += "0";
            j = new_message.length();
        }
        new_message = new_message + message_bit;
    }
    num_of_zeros = j;
    return new_message;
}

string encryption(string message , string key){
    string cipher ,mbits;
    mbits = ASCIIToBinary(message);
    string initial_permu = permute(mbits, IP, 64);

    string L[17];
    string R[17];

    L[0] = initial_permu.substr(0, 32);
    R[0] = initial_permu.substr(32, 32);


    string* ptrKeys; //pointer to hold address
	ptrKeys = generate_subkeys(key);
    for (int i = 1; i<17; i++)
    {
        L[i] = R[i - 1];
        R[i] = xor_(L[i - 1], f(R[i-1], ptrKeys[i]));
    }

    cipher = permute(R[16] + L[16], FP, 64);/*result of Final permutation == Cipher in bits */

    return cipher;
}

string decryption(string cipher, string key)
{
    string* ptrKeys; //pointer to hold address
	ptrKeys = generate_subkeys(key);

    string newkeys[17];
    int j = 16;

    for (int i = 0; i <17; i++)/* for loop to store keys in reverse order */
    {
        newkeys[i] = ptrKeys[j];
        j--;
    }

    string L[17];
    string R[17];
    string initial_permu = permute(cipher, IP, 64);
    L[0] = initial_permu.substr(0, 32);
    R[0] = initial_permu.substr(32, 32);

    for (int i = 1; i < 17; i++)
    {
        L[i] = R[i - 1];
        R[i] = xor_(L[i - 1], f(R[i - 1], newkeys[i-1]));
    }

    string p = permute(R[16] + L[16], FP, 64);/*result of Final permutation == Cipher in bits */

    return p;
}


int main()
{
    cout << "** DES Encryption **" << endl;
    string message, key;
    cout<< "\nEnter Your Message: ";
    getline(cin ,message);
    cout<< "\nEnter Your Key: ";
    getline(cin ,key);

    while(key.length() != 8)
    {
        cout<<"\nError!! You Must Enter key as 8 characters!! "<<endl;
        cout<< "\nEnter Your Key: ";
        getline(cin ,key);
    }/* for loop to store keys in reverse order */

    string new_message = handle_block_of_64(message);

    int size = new_message.length() / 64;

    string * block = new string[size];

    string cipher = "";
    string plaintxt= "";

    for (int i = 0; i < size; i++)
    {
        block[i] = new_message.substr(i*64 , 64) ;
        cipher += encryption(BinaryToASCII(block[i]),key);
    }

    for (int i = 0; i < size; i++)
    {
        plaintxt += decryption(cipher.substr(i*64 ,64),key);
    }

    cout << "\nCipher Text is : " << BinaryToASCII(cipher) <<endl;

    cout<<"\n** DES Decryption **"<<endl;

    plaintxt = plaintxt.substr(num_of_zeros , plaintxt.length()- num_of_zeros + 1);

    plaintxt = BinaryToASCII(plaintxt);

    cout<<"\nPlain Text is : "<<  plaintxt<< endl;

    return 0;
}
