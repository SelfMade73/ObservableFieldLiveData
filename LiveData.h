/// @file Observable field

#ifndef LIVE_DATA_LIVEDATA_H
#define LIVE_DATA_LIVEDATA_H

#include <functional>
#include <vector>


namespace live_data
{

/// @brief Interface for observers
template<typename T>
class IObserver{

public:
    /// @brief Method executes when data changes
    /// @param data Updated data
    virtual void Observe( const T& data) = 0;

    virtual ~IObserver() = default;
};



/// @brief Observable field class
template<typename T>
class LiveData final
{
public:
    /// @brief Callback type
    using OnChangedCallback = std::function<void(const T&)>;

    template<typename ... Args>
    explicit LiveData(Args&&... args) : data_(  T(std::forward<Args>(args)...)){}
    LiveData() = default;

    /// @brief Add callback
    /// @param callback On value changed callback
    inline void addObserver( const LiveData::OnChangedCallback& callback) noexcept;

    /// @brief Add observer
    /// @param observer Observer for value
    inline void addObserver( std::shared_ptr<IObserver<T>> observer) noexcept;

    /// @brief Setter for value
    /// @details Runs all callbacks
    /// @param data_ input data
    template<typename U>
    inline void setValue( U&& data_ );

    /// @brief Setter for value without notifying
    /// @param data_ input data
    template<typename U>
    inline void setValueQuietly( U&& data ) noexcept;

    /// @brief Getter for value
    /// @return inner data
    inline T& Get() noexcept;

    /// @brief Calls set value
    /// @param data_ input data
    /// @return this object
    inline LiveData<T>& operator= ( const T& data) noexcept;

    /// @brief Removes all callbacks and all observers
    inline void clearCallbacksAndObservers() noexcept;
private:

    /// @brief Runs all callbacks and notifies all observers
    inline void notifyAll();

private:
    std::vector<OnChangedCallback> callbacks;
    std::vector<std::shared_ptr<IObserver<T>>> observers;
    T data_;
};


template<typename T>
void LiveData<T>::addObserver( const LiveData::OnChangedCallback& callback) noexcept {
    callbacks.push_back(callback);
}

template<typename T>
void LiveData<T>::notifyAll() {
    for ( const auto& it : callbacks){
        it(data_);
    }
    for ( std::shared_ptr<IObserver<T>> it : observers){
        if (it){
            it->Observe(data_);
        }
    }
}

template<typename T>
template <typename U>
void LiveData<T>::setValue( U&& data) {
    data_ = std::forward<U>(data);
    notifyAll();
}

template<typename T>
T& LiveData<T>::Get() noexcept {
    return data_;
}

template<typename T>
void LiveData<T>::clearCallbacksAndObservers() noexcept {
    callbacks.clear();
    observers.clear();
}

template<typename T>
template <typename U>
void LiveData<T>::setValueQuietly(U &&data) noexcept {
    data_ = std::forward<U>(data);
}



template<typename T>
LiveData<T>& LiveData<T>::operator=(const T &data) noexcept {
    setValue(data);
    return *this;
}

template<typename T>
void LiveData<T>::addObserver(std::shared_ptr<IObserver<T>> observer) noexcept {
    observers.push_back(observer);
}


}


#endif //LIVE_DATA_LIVEDATA_H
