#include "vector.h"
using namespace nstl;

std::allocator<std::string> vector::alloc;

vector::vector(const vector& str) {
	auto data = alloc_n_copy(str.begin(), str.end());
	elements = data.first;
	first_free = cap = data.second;
}

vector::vector(vector&& str) noexcept : elements(str.elements) , first_free(str.first_free), cap(str.cap) {
	str.elements = str.first_free = str.cap = nullptr;
}

vector::~vector() {
	free();
}

vector& vector::operator= (const vector& str) & {
	auto data = alloc_n_copy(str.begin(), str.end());
	free();
	elements = data.first;
	first_free = cap = data.second;
	return *this;
}

vector& vector::operator= (vector&& str) noexcept {
	if (this != &str) {
		free();
		elements = str.elements;
		first_free = str.first_free;
		cap = str.cap;
		str.elements = str.first_free = str.cap = nullptr;
	}
	return *this;
}

void vector::push_back(const std::string& str) {
	chk_n_alloc();
	alloc.construct(first_free++, str);
}

void vector::push_back(std::string&& str) {
	chk_n_alloc();
	alloc.construct(first_free++, std::move(str));
}

std::pair<std::string*, std::string*> vector::alloc_n_copy(const std::string* begin, const std::string* end) {
	auto data = alloc.allocate(end - begin);
	//warning C4996: Call to 'std::uninitialized_copy' with parameters that may be unsafe
	return{ data, std::uninitialized_copy(begin, end, data) };
}

void vector::free() {
	if (elements) {
		for (auto p = first_free; p != elements; ) {
			alloc.destroy(--p);
		}
		alloc.deallocate(elements, cap - elements);
	}
}

void vector::reallocate() {
	auto capacity = size() ? 2 * size() : 1;
	auto first = alloc.allocate(capacity);
	auto last = std::uninitialized_copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), first);
	elements = first;
	first_free = last;
	cap = elements + capacity;
}

/*void vector::reallocate() {
	auto capacity = size() ? 2 * size() : 1;
	auto data = alloc.allocate(capacity);
	auto databak = data;
	auto elembak = elements;
	for (size_t i = 0; i != size(); ++i) {
		//using std::move(string,don't copy construct)
		alloc.construct(databak++, std::move(*elembak++));
	}
	free(); //need elemets, no destructor string
	elements = data;
	first_free = databak;
	cap = elements + capacity;
}*/