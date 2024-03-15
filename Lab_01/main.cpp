#include <iostream>

using llu = long long unsigned int;

const int MAX_COUNT_SORT_VALUE = 255;
const int ONE_BYTE_COUNT = 1;
const int TWO_BYTE_COUNT = 2;
const int BITS_IN_ONE_BYTE = 8;
const int DATE_MAX_LENGTH = 11;

template<typename T>
class TVector {
    private:
        int Size;
        int Capacity;
        T* Arr;

        void Reallocate(int);
    public:
        TVector();
        TVector(int);

        void PushBack(const T&);
        void PopBack();
        void Erase(int);
        void Clear();

        int GetSize();
        int GetCapacity();

        T& operator[] (int) const;
        TVector<T>& operator = (const TVector<T>&);

        ~TVector();
};

template<typename T>
void TVector<T>::Reallocate(int newCapacity) {
    if (Arr == nullptr) {
        return;
    }

    T* temp = new T[Size];
    for (int i = 0; i < Size; ++i) {
        temp[i] = Arr[i];
    }

    delete[] Arr;
    Arr = new T[newCapacity];

    for (int i = 0; i < Size; ++i) {
        Arr[i] = temp[i];
    }

    Capacity = newCapacity;
    delete[] temp;
}

template<typename T>
TVector<T>::TVector() {
    Size = 0;
    Capacity = 2;
    Arr = new T[Capacity];
}

template<typename T>
TVector<T>::TVector(int size) {
    Size = size;

    if (Size > 0) {
        Capacity = Size * 2;
    } else {
        Capacity = 2;
    }

    Arr = new T[Capacity];
}

template<typename T>
void TVector<T>::PushBack(const T& elem) {
    if (Size >= Capacity) {
        this->Reallocate(Capacity * 2);
    }

    Arr[Size++] = elem;
}

template<typename T>
void TVector<T>::PopBack() {
    if (!Size) {
        return;
    }

    Size--;
}

template<typename T>
void TVector<T>::Erase(int index) {
    if (index >= Size) {
        return;
    }

    for (int i = index + 1; i < Size; ++i) {
        Arr[i - 1] = Arr[i];
    }

    Size--;
}

template<typename T>
void TVector<T>::Clear() {
    Size = 0;
}

template<typename T>
T& TVector<T>::operator[] (int index) const {
    if (index >= Size) {
        throw std::logic_error("Too big index");
    }

    return Arr[index];
}

template<typename T>
TVector<T>& TVector<T>::operator = (const TVector<T>& other) {
    delete[] Arr;
    Size = other.Size;
    Capacity = other.Capacity;
    Arr = new T[Capacity];

    for (int i = 0; i < Size; ++i) {
        Arr[i] = other[i];
    }

    return *this;
}

template<typename T>
int TVector<T>::GetSize() {
    return Size;
}

template<typename T>
int TVector<T>::GetCapacity() {
    return Capacity;
}

template<typename T>
TVector<T>::~TVector() {
    Size = 0;
    Capacity = 0;
    delete[] Arr;
    Arr = nullptr;
}

struct TSortObject {
    int Day;
    int Month;
    int Year;
    llu Value;
    char DateInString[DATE_MAX_LENGTH];

    TSortObject() = default;
    TSortObject(int, int, int, llu, char*);

    void Parse();

    TSortObject& operator = (const TSortObject&);

    ~TSortObject() = default;
};

TSortObject::TSortObject(int day, int month, int year, llu value, char* date) {
    Day = day;
    Month = month;
    Year = year;
    Value = value;

    for (int i = 0; i < DATE_MAX_LENGTH; ++i) {
        DateInString[i] = date[i];
    }
}

void TSortObject::Parse() {
    int dotCount = 0;
    Day = 0;
    Month = 0;
    Year = 0;

    for (int i = 0; i < DATE_MAX_LENGTH; ++i) {
        if (DateInString[i] == '\0') {
            break;
        }

        if (DateInString[i] >= '0' && DateInString[i] <= '9') {
            switch (dotCount) {
                case 0:
                    Day = Day * 10 + (DateInString[i] - '0');
                    break;
                case 1:
                    Month = Month * 10 + (DateInString[i] - '0');
                    break;
                case 2:
                    Year = Year * 10 + (DateInString[i] - '0');
                    break;
                default:
                    throw std::logic_error("Bad parsing");
                    break;
            }
        } else if (DateInString[i] == '.') {
            ++dotCount;
        }
    }
}

TSortObject& TSortObject::operator = (const TSortObject& other) {
    Day = other.Day;
    Month = other.Month;
    Year = other.Year;
    Value = other.Value;

    for (int i = 0; i < DATE_MAX_LENGTH; ++i) {
        DateInString[i] = other.DateInString[i];
    }

    return *this;
}

struct TTriplet {
    TSortObject First;
    int Second;
    int Third;

    TTriplet() = default;
    TTriplet(TSortObject&, int, int);

    TSortObject& GetFirst();
    int GetSecond();
    int GetThird();

    TTriplet& operator = (const TTriplet&);

    ~TTriplet() = default;    
};

TTriplet::TTriplet(TSortObject& obj, int value, int temp) {
    First = obj;
    Second = value;
    Third = temp;
}

TSortObject& TTriplet::GetFirst() {
    return First;
}

int TTriplet::GetSecond() {
    return Second;
}

int TTriplet::GetThird() {
    return Third;
}

TTriplet& TTriplet::operator = (const TTriplet& other) {
    First = other.First;
    Second = other.Second;
    Third = other.Third;

    return *this;
}

void CountSort(TVector<TTriplet>& values) {
    int array[MAX_COUNT_SORT_VALUE + 1];
    TVector<TTriplet> temp(values.GetSize());

    for (int i = 0; i <= MAX_COUNT_SORT_VALUE; ++i) {
        array[i] = 0;
    }

    for (int i = 0; i < values.GetSize(); ++i) {
        ++array[values[i].Third];
    }

    for (int i = 1; i <= MAX_COUNT_SORT_VALUE; ++i) {
        array[i] += array[i - 1];
    }

    for (int i = values.GetSize() - 1; i >= 0; --i) {
        temp[array[values[i].Third] - 1] = values[i];
        --array[values[i].Third];
    }

    values = temp;
}

void RadixSort(TVector<TTriplet>& values, int bytesCount) {
    for (int i = 0; i < bytesCount; ++i) {
        for (int j = 0; j < values.GetSize(); ++j) {
            values[j].Third = (values[j].Second >> (BITS_IN_ONE_BYTE * i)) & MAX_COUNT_SORT_VALUE;
        }

        CountSort(values);
    }
}

void Sort(TVector<TTriplet>& vec) {
    for (int i = 0; i < vec.GetSize(); ++i) {
        vec[i].Second = vec[i].First.Day;
    }
    RadixSort(vec, ONE_BYTE_COUNT);

    for (int i = 0; i < vec.GetSize(); ++i) {
        vec[i].Second = vec[i].First.Month;
    }
    RadixSort(vec, ONE_BYTE_COUNT);

    for (int i = 0; i < vec.GetSize(); ++i) {
        vec[i].Second = vec[i].First.Year;
    }
    RadixSort(vec, TWO_BYTE_COUNT);
}

int main() {
    TVector<TTriplet> vec;
    TSortObject temp;

    while (scanf("%s\t%llu", temp.DateInString, &temp.Value) != -1) {
        temp.Parse();
        vec.PushBack({temp, 0, 0});
    }

    Sort(vec);

    for (int i = 0; i < vec.GetSize(); ++i) {
        for (int j = 0; j < DATE_MAX_LENGTH; ++j) {
            if (vec[i].First.DateInString[j] == '\0') {
                break;
            }
            std::cout << vec[i].First.DateInString[j];
        }
        std::cout << '\t' << vec[i].First.Value << '\n';
    }

    return 0;
}