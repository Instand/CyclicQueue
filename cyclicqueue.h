#ifndef CYCLICQUEUE_H
#define CYCLICQUEUE_H

#include <queue>
#include <vector>

    //aggregate class
    template <typename Type,
              typename Sequence = std::deque<Type> >
    class CyclicQueue {
        public:

            typedef typename std::queue<Type>::value_type           value_type;
            typedef typename std::queue<Type>::reference            reference;
            typedef typename std::queue<Type>::const_reference      const_reference;
            typedef typename std::queue<Type>::size_type            size_type;
            typedef Sequence                                        container_type;

            explicit CyclicQueue(unsigned int size) : baseQ(),
                _cyclicMaxCount(size) {
                fill();
            }

            explicit CyclicQueue() : baseQ(),
                _cyclicMaxCount() {}

            ~CyclicQueue() {
                clear();
            }

            explicit CyclicQueue(const CyclicQueue& cQ) {
                baseQ = cQ.baseQ;
                _cyclicMaxCount = cQ._cyclicMaxCount;
            }

            template<typename T, typename Seq>
            friend bool operator==(const CyclicQueue<T, Seq>&, const CyclicQueue<T,Seq>&);

            template<typename T, typename Seq>
            friend bool operator<(const CyclicQueue<T, Seq>&, const CyclicQueue<T, Seq>&);

            void fill () {
                clear();

                for (unsigned i = 0; i < _cyclicMaxCount; ++i) {
                    Type* t = new Type();
                    baseQ.push(*t);
                    delete t;
                }
            }

            void swap(CyclicQueue& cQ) {
                baseQ.swap(cQ.baseQ);
                std::swap(maxCount(), cQ.maxCount());
            }

            unsigned maxCount() const {
                return _cyclicMaxCount;
            }

            std::vector<Type> toStdVector() {
                std::vector<Type> _vector;
                std::queue<Type, Sequence> _queue = baseQ;

                while (!baseQ.empty()) {
                    _vector.insert(_vector.begin(), baseQ.front());
                    baseQ.pop();
                }

                baseQ.swap(_queue);

                return _vector;
            }

            bool fromStdVector(std::vector<Type>& _vector) {

                if (_vector.size() > maxCount())
                    return false;

                fill();

                for (typename std::vector<Type>::reverse_iterator iter = _vector.rbegin(); iter != _vector.rend(); ++iter)
                    baseQ.push(*iter);

                return true;
            }

            void clear() {
                while(!baseQ.empty()) baseQ.pop();
            }

            void changeMaxCount(unsigned _max) {
                _cyclicMaxCount = _max;
                fill();
            }

            bool empty() const {
                return baseQ.empty();
            }

            size_type size() const {
                return baseQ.size();
            }

            reference front() {
                return  baseQ.front();
            }

            reference element() {
                return baseQ.back();
            }

            void push(const Type& elem) {
                if (baseQ.size() < maxCount())
                    baseQ.push(elem);
                else {
                    baseQ.pop();
                    baseQ.push(elem);
                }
            }

            void pop() {
                if (!baseQ.empty()) baseQ.pop();
            }


        private:
            std::queue<Type, Sequence> baseQ;
            unsigned int _cyclicMaxCount;
    };

    template<typename T, typename Seq>
    inline bool operator==(const CyclicQueue<T, Seq>& ls, const CyclicQueue<T,Seq>& rs) {
        return ls.baseQ == rs.baseQ;
    }

    template<typename T, typename Seq>
    inline bool operator<(const CyclicQueue<T, Seq>& ls, const CyclicQueue<T, Seq>& rs) {
        return ls.baseQ < rs.baseQ;
    }


#endif // CYCLICQUEUE_H
