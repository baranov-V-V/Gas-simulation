#ifndef MATH_VECTOR_2D
#define MATH_VECTOR_2D

template <typename T>
class MathVector2D
{   
public:
    void set_x(T new_x) {x = new_x;};
    void set_y(T new_y) {y = new_y;};

    T get_x() { return x; };
    T get_y() { return y; };

    T get_x() const { ((MathVector2D*)this)->get_x(); };
    T get_y() const { ((MathVector2D*)this)->get_y(); };

    MathVector2D() = default;
    MathVector2D(const T& x, const T& y) : x(x), y(y) {};

    MathVector2D& operator+=(const MathVector2D& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    MathVector2D& operator-=(const MathVector2D& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    MathVector2D& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    void print(std::ostream& stream) { stream << "(" << x << ", " << y << ")"; }
    void print() { this->print(std::cout); };
    
    void print(std::ostream& stream) const { ((MathVector2D*)this)->print(stream); }
    void print() const { this->print(std::cout); };

    double length() { return sqrtf(x * x + y * y); };

private:
    T x;
    T y;
};

template <typename T>
MathVector2D<T> operator+(const MathVector2D<T>& lhs, const MathVector2D<T>& rhs) {
    MathVector2D<T> result(lhs);
    result += rhs;
    return result;
}

template <typename T>
MathVector2D<T> operator-(const MathVector2D<T>& lhs, const MathVector2D<T>& rhs) {
    MathVector2D<T> result(lhs);
    result -= rhs;
    return result;
}

template <typename T>
MathVector2D<T> operator*(const MathVector2D<T>& vector, double scalar) {
    MathVector2D<T> result(vector);
    result *= scalar;
    return result;
}

template <typename T>
double operator*(const MathVector2D<T>& lhs, const MathVector2D<T>& rhs) {
    return lhs.get_x() * rhs.get_x() + lhs.get_y() * rhs.get_y();
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const MathVector2D<T>& vector) {
    vector.print(stream);
    return stream;
}

#endif