#include <iostream>
#include "mystring.hpp"
#include <cstring>

//! допоміжний конструктор
my_str_t::my_str_t(size_t const capacity):
        capacity_m(capacity * 2), size_m(0)
{
    data_m = new char[capacity_m];
    data_m[0] = '\0';
}

//! Створює стрічку із size копій літери initial
my_str_t::my_str_t(size_t size, char initial):
    size_m(size), capacity_m(size*2) {
    data_m = new char[capacity_m]; //! виділили память довжиною capacity_m
    std::memset(data_m, initial, size); //!memset заповнює перші
    //! size елементів масиву data_m символом initial
    data_m[size_m] = '\0'; //! на позицію останнього символа '\0', щоб була С стрічка

    // std::cout << "Content: " << data_m << std::endl;
    // std::cout << "Size: " << size_m << std::endl;
    // std::cout << "Capacity: " << capacity_m << std::endl;
}

//! Копіює вміст С-стрічки, вимоги до capacity_m -- ті ж, що вище
my_str_t::my_str_t(const char* cstr) {
    //! const char* cstr = поінтер на незмінний С-шний масив букв.
    size_m = std::strlen(cstr); //!обчислює довжину стрічки cstr, не враховуючи нульовий символ завершення
    data_m = new char[size_m]; //! виділили память довж size_m
    capacity_m = size_m+1; //! враховуємо символ завершення
    std::strcpy(data_m, cstr); //! перекопіювали
    // std::cout << "Size: " << size_m << std::endl;
    // std::cout << "Capacity: " << capacity_m << std::endl;
    // std::cout << "Content: " << data_m << std::endl;
}

//! Копіює стрічку С++, вимоги до capacity_m -- ті ж, що вище
my_str_t::my_str_t(const std::string& str) {
    size_m = str.size();    // Зберігаємо розмір стрічки
    capacity_m = size_m + 1;    // Виділяємо місце під символ '\0'
    data_m = new char[capacity_m];  // Виділяємо пам'ять
    std::strcpy(data_m, str.c_str());   // Копіюємо вміст std::string у data_m
    data_m[size_m] = '\0';
    // std::cout << "Size: " << size_m << std::endl;
    //std::cout << "Capacity: " << capacity_m << std::endl;
    //std::cout << "Content: " << data_m << std::endl;
}

//! конструктор копій
my_str_t::my_str_t(const my_str_t & mystr):
    capacity_m(mystr.capacity_m),size_m(mystr.size_m){  // ініціалізували
    data_m = new char[capacity_m];  // виділили память
    for (size_t i = 0; i < size_m; i++) { //посимвольно перекопіювали
        data_m[i] = mystr.data_m[i];}
    data_m[size_m] = '\0'; // символ завершення стрінги
}

//!оператор присвоєння
my_str_t& my_str_t::operator=(const my_str_t& mystr) {
    if (this == &mystr) {
        return *this; // Перевірка на самоприсвоєння
    }
    delete[] data_m; // Звільняємо стару пам'ять
    size_m = mystr.size_m;
    capacity_m = mystr.capacity_m;
    data_m = new char[capacity_m]; // Виділяємо нову пам'ять
    std::memcpy(data_m, mystr.data_m, size_m); // Копіюємо дані
    return *this; // Повертаємо посилання на поточний об'єкт
}

void my_str_t::swap(my_str_t& other) noexcept
{
    char* const temp = data_m;
    data_m = other.data_m;
    other.data_m = temp;
    size_t const temp2 = size_m;
    size_m = other.size_m;
    other.size_m = temp2;
    size_t const temp3 = capacity_m;
    capacity_m = other.capacity_m;
    other.capacity_m = temp3;
}

//! непостійний оператор індексації, повертає посилання на символ,
//! що можна змінити.
char& my_str_t::operator[](size_t idx) {
    if (idx>= size_m)
    {throw std::out_of_range("Incorrect index!");}
    return data_m[idx];
}

//! постійний оператор індексування,що використовується для постійних обєктів
const char& my_str_t::operator[](size_t idx) const {
    if (idx>= size_m)
    {throw std::out_of_range("Incorrect index!");}
    return data_m[idx];
}

char& my_str_t:: at(size_t idx){
    if (idx >= size_m) {
        throw std::out_of_range("Incorrect index!");
    }
    return data_m[idx];
}

const char& my_str_t:: at(size_t idx) const {
    if (idx >= size_m) {
        throw std::out_of_range("Incorrect index!");
    }
    return data_m[idx];
}

//! Збільшує capacity до new_capacity.
//! Якщо new_capacity менша за capacity_m -- не робить нічого
//! Для збільшення -- виділяє новий блок пам'яті,
//! копіює size_m байт -- фактичний вміст стрічки,
//! у цей новий блок, звільняє старий блок, а
//! вказівник на новий зберігає в data_m.
//! Обробка помилок:
//! Не повинні заважати пропагуванню виключення
//! std::bad_alloc.
void my_str_t::reserve(size_t new_capacity){
    if (new_capacity > capacity_m){
        char* new_data_m = new char[new_capacity];
        std::memcpy(new_data_m, data_m, size_m);
        delete[] data_m;
        capacity_m = new_capacity;
        data_m = new_data_m;
    }
}

void my_str_t:: shrink_to_fit(){
    if (size_m > capacity_m) {
        throw std::out_of_range("capacity out of range");}
    char* data = new char[size_m];
    std::memcpy(data, data_m, size_m);
    delete[] data_m;
    data_m = data;
    capacity_m = size_m;
}

void my_str_t::resize(size_t new_size, char new_char)
{
    if (new_size <= size_m){
        size_m = new_size;
        data_m[size_m] = '\0';
    }
    else
    {
        if (new_size > capacity_m){
            this->reserve(new_size * 2);}
        for(size_t i = size_m; i < new_size; i++){
            data_m[i] = new_char;}
        size_m = new_size;
        data_m[size_m] = '\0';
    }
}

void my_str_t::clear() {
    size_m = 0;     //! розмір стрінги = 0
    data_m[size_m] = '\0';  //! + символ завершення
}


void my_str_t::insert(size_t idx, const my_str_t& str) {
    if (idx > size_m) {
        throw std::out_of_range("index out of range");
    }

    if (size_m + str.size_m > capacity_m) {
        this->shrink_to_fit();
        char* data = new char[size_m + str.size_m + 1];

        std::memmove(data, data_m, idx);                      // Копіюємо першу частину
        std::memmove(data + idx, str.data_m, str.size_m);      // Вставляємо новий рядок
        std::memmove(data + idx + str.size_m, data_m + idx, size_m - idx); // Копіюємо решту

        delete[] data_m;
        data_m = data;
        capacity_m = size_m + str.size_m + 1;
    }

    size_m += str.size_m;  // Оновлюємо фактичний розмір
}

void my_str_t::insert(size_t idx, char c) {
    if (idx > size_m) {
        throw std::out_of_range("index out of range");
    }

    // this->shrink_to_fit();
    char* data = new char[2*size_m +1];  // Виділяємо місце під новий символ

    std::memmove(data, data_m, idx);    // Копіюємо першу частину
    data[idx] = c;                      // Вставляємо символ
    std::memmove(data + idx + 1, data_m + idx, size_m - idx); // Копіюємо решту

    delete[] data_m;
    data_m = data;
    capacity_m = 2*size_m +1;  // Оновлюємо ємність
    size_m += 1;              // Оновлюємо фактичний розмір
}


void my_str_t::insert(size_t idx, const char* cstr) {
    size_t len = std::strlen(cstr);

    if (idx > size_m) {
        throw std::out_of_range("index out of range");
    }

    shrink_to_fit();
    char* data = new char[size_m + len + 1];  // Виділяємо місце під новий рядок

    std::memmove(data, data_m, idx);          // Копіюємо першу частину
    std::memmove(data + idx, cstr, len);      // Вставляємо новий рядок
    std::memmove(data + idx + len, data_m + idx, size_m - idx); // Копіюємо решту

    delete[] data_m;
    data_m = data;
    capacity_m = size_m + len + 1;  // Оновлюємо ємність
    size_m += len;                  // Оновлюємо фактичний розмір
}

void my_str_t::append(char c)
{
    if (size_m + 1 == capacity_m)
    {
        this->reserve(capacity_m * 2);
    }
    data_m[size_m] = c;
    data_m[size_m + 1] = '\0';
    ++size_m;
}

void my_str_t::append (const my_str_t& str) {
    insert(size_m, str);
}

void my_str_t::append (const char* cstr) {
    insert(size_m, cstr);
}

void my_str_t::erase(size_t begin, size_t size) {
    if (begin >= size_m) {
        throw std::out_of_range("index out of range");
    }
    int i = 0;
    while (begin < size_m && i < size) {
        std::memmove(data_m + begin, data_m + begin + 1, size_m - begin);
        i++;
        size_m--;
    }
}

const char* my_str_t::c_str() const { return data_m; }
size_t  my_str_t::capacity() const  { return capacity_m; }
size_t my_str_t::size() const { return size_m; }

size_t my_str_t::find(char c, size_t idx)
{
    if (idx >= size_m)
    {
        throw std::out_of_range("my_str_t::find");
    }
    for (size_t i = idx; i < size_m; ++i)
    {
        if (data_m[i] == c)
        {
            return i;
        }
    }
    return not_found;
}

size_t my_str_t::find(const std::string& str, size_t idx)
{
    size_t substr_i = 0;
    size_t str_len = str.size();
    for (size_t i = idx; i < size_m; ++i)
    {
        while (substr_i < str_len && data_m[i + substr_i] == str[substr_i])
        {
            ++substr_i;
        }
        if (substr_i == str_len)
        {
            return i;
        }
        substr_i = 0;
    }
    return not_found;
}

size_t my_str_t::find(const char* cstr, size_t idx)
{
    size_t substr_i = 0;
    size_t str_len = std::strlen(cstr);
    std::cout << str_len << std::endl;
    for (size_t i = idx; i < size_m; ++i)
    {
        while (substr_i < str_len && data_m[i + substr_i] == cstr[substr_i])
        {
            ++substr_i;
        }
        if (substr_i == str_len)
        {
            return i;
        }
        substr_i = 0;
    }
    return not_found;
}

my_str_t my_str_t::substr(size_t begin, size_t size)
{
    if (begin >= size_m)
    {
        throw std::out_of_range("my_str_t::substr");
    }
    my_str_t substr = my_str_t(size);
    std::memcpy(substr.data_m, &(this->at(begin)), size);
    return substr;
}

//! ~my_str_t();
my_str_t::~my_str_t() {
    delete[] data_m;
}

//!<< оператор вивести в консоль
std::ostream& operator<<(std::ostream& stream, const my_str_t& str) {
    stream << str.c_str(); //! викликали гетер для data_m
    return stream; }

std::istream& operator>>(std::istream& stream, my_str_t& str){
    while (std::isspace(stream.get())){}
    stream.unget();
    str.clear();
    while (!std::isspace(stream.peek()))
    {
        str.append(static_cast<char>(stream.get()));
    }
    return stream;
}

std::istream& readline(std::istream& stream, my_str_t& str)
{
    str.clear();
    for (size_t i = 0; stream.peek() != '\n'; i++)
    {
        str.append(stream.get());
    }
    return stream;
}

//! приймає на всіх 2 обєкти мого класу і порівнює
bool operator==(const my_str_t &str1, const my_str_t &str2) {
    if (str1.size() != str2.size()) { return false; }
    for (size_t i = 0; i < str1.size(); i++) {
        if (str1[i] != str2[i]) { return false; }
    }
    return true;
}

//! приймає на вхід 1 обєкт мого класу і 1 с стрічку
bool operator==(const my_str_t& str1, const char* cstr2){
    if (str1.size() != std::strlen(cstr2)) { return false; }
    for (size_t i = 0; i < str1.size(); i++) {
        if (str1[i] != cstr2[i]) { return false; }
    }
    return true;
}

//! приймає на вхід 1 c string  і 1 екземпляр мого класу
bool operator==(const char* cstr1, const my_str_t& str2) {
    if (std::strlen(cstr1) != str2.size()) { return false; }
    for (size_t i = 0; i < str2.size(); i++) {
        if (cstr1[i] != str2[i]) { return false; }
    }
    return true;}


bool operator!=(const my_str_t& str1, const my_str_t& str2) {
    return !(str1==str2);
}

bool operator!=(const my_str_t& str1, const char* cstr2){
    return !(str1==cstr2);
}

bool operator!=(const char* cstr1, const my_str_t& str2){
    return !(cstr1==str2);
}

bool operator< (const my_str_t& str1, const my_str_t& str2) {
  if (str1.size() == str2.size()) {
    int i = 0;
    while (str1.c_str()[i] == str2.c_str()[i] && i < str1.size()) {
      i++;
      }
    if (i < str1.size()) {
      return str1.c_str()[i] < str2.c_str()[i];
    }
    else {return false;}
  }
  return str1.size() < str2.size();
}

bool operator< (const my_str_t& str1, const char* cstr2) {
  if (str1.size() == strlen(cstr2)) {
    int i = 0;
    while (str1.c_str()[i] == cstr2[i] && i < str1.size()) {
      i++;
    }
    if (i < str1.size()) {
      return str1.c_str()[i] < cstr2[i];
    }
    else {return false;}
  }
  return str1.size() < strlen(cstr2);
}

bool operator< (const char* cstr1, const my_str_t& str2) {
  return (str2 > cstr1);
}

bool operator> (const my_str_t& str1, const my_str_t& str2) {
  return (str2 < str1);
}

bool operator> (const my_str_t& str1, const char* cstr2) {
  if (str1.size() == strlen(cstr2)) {
    int i = 0;
    while (str1.c_str()[i] == cstr2[i] && i < str1.size()) {
      i++;
    }
    if (i < str1.size()) {
      return str1.c_str()[i] > cstr2[i];
    }
    else {return false;}
  }
  return str1.size() > strlen(cstr2);
}

bool operator> (const char* cstr1, const my_str_t& str2) {
  return (str2 < cstr1);
}

bool operator<= (const my_str_t& str1, const my_str_t& str2) {
  if (str1.size() == str2.size()) {
    int i = 0;
    while (str1.c_str()[i] == str2.c_str()[i] && i < str1.size()) {
      i++;
    }
    if (i < str1.size()) {
      return str1.c_str()[i] < str2.c_str()[i];
    }
    else {return true;}
  }
  return str1.size() <= str2.size();
}

bool operator<= (const my_str_t& str1, const char* cstr2) {
  if (str1.size() == strlen(cstr2)) {
    int i = 0;
    while (str1.c_str()[i] == cstr2[i] && i < str1.size()) {
      i++;
    }
    if (i < str1.size()) {
      return str1.c_str()[i] < cstr2[i];
    }
    else {return true;}
  }
  return str1.size() <= strlen(cstr2);
}

bool operator<= (const char* cstr1, const my_str_t& str2) {
  return (str2 >= cstr1);
}

bool operator>= (const my_str_t& str1, const my_str_t& str2) {
  return (str2 <= str1);
}

bool operator>=(const my_str_t& str1, const char* cstr2) {
  if (str1.size() == strlen(cstr2)) {
    int i = 0;
    while (str1.c_str()[i] == cstr2[i] && i < str1.size()) {
      i++;
    }
    if (i < str1.size()) {
      return str1.c_str()[i] > cstr2[i];}
    else {return true;}
  }
  return str1.size() >= strlen(cstr2);
}

bool operator>= (const char* cstr1, const my_str_t& str2) {
  return (str2 <= cstr1);
}

int main() {
    // Ваш код тут
    return 0;
}
