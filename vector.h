#ifndef NSTL_VECTOR_H_
#define NSTL_VECTOR_H_
#include <memory>
#include <string>

namespace nstl {
	class vector {
	public:
		vector() : elements(nullptr), first_free(nullptr), cap(nullptr) {
		}
		vector(const vector&);
		vector(vector&&) noexcept;
		vector& operator= (const vector&) &;
		vector& operator= (vector&&) noexcept;
		~vector();
		void push_back(const std::string&);
		void push_back(std::string&&);
		size_t size() const {
			return first_free - elements;
		}
		size_t capacity() const {
			return cap - elements;
		}
		std::string* begin() const {
			return elements;
		}
		std::string* end() const {
			return first_free;
		}
	private:
		static std::allocator<std::string> alloc;
		void chk_n_alloc() {
			if (size() == capacity()) {
				reallocate();
			}
		}
		std::pair<std::string*, std::string*> alloc_n_copy(const std::string*, const std::string*);
		void free();
		void reallocate();
		std::string* elements;
		std::string* first_free;
		std::string* cap;
	};
}
#endif