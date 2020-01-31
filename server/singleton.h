template <typename T>
class Singleton {
public:
    // ...

private:
    static T *mpInstance;
};

template <typename T>
T *Singleton<T>::mpInstance;
