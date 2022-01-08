#ifndef UNIQUE_PTR_REF_HPP
#define UNIQUE_PTR_REF_HPP

namespace ren{
    template <typename T> class unique_ptr_ref
    {
        public:
            unique_ptr_ref(std::unique_ptr<T>& RendorObj)
            {
                m_RendorPtr = RendorObj.get();
            }
            unique_ptr_ref() = default;

            T& operator*()
            {
                return *m_RendorPtr;
            }

            T* operator->()
            {    
                return m_RendorPtr;
            }

            bool IsValid() 
            {
                return m_RendorPtr != nullptr;
            }
        
        private:
            T* m_RendorPtr;
    };
}

#endif // UNIQUE_PTR_REF_HPP