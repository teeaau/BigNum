#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class BigNum {
    private:
        vector<long long> _beforePoint;
        vector<long long> _afterPoint;
        string _displayNumber;
        bool _positive;
        void ToDisplay();
        void ToNumber();
        void FixDisplay();
        void FixNumber();
        void FillZeros(BigNum& _num);
        BigNum Inverse();
    public:
        BigNum();
        BigNum(int _num);
        BigNum(long long _num);
        BigNum(double _num);
        BigNum(const BigNum& _num);
        BigNum(string _error);
        bool operator == (const auto& _num);
        bool operator != (const auto& _num);
        bool operator > (const auto& _num);
        bool operator >= (const auto& _num);
        bool operator < (const auto& _num);
        bool operator <= (const auto& _num);
        BigNum operator+(const auto& _num);
        void operator+=(const auto& _num);
        BigNum operator-();
        BigNum operator-(const auto& _num);
        void operator-=(const auto& _num);
        BigNum operator*(const auto& _num);
        void operator*=(const auto& _num);
        BigNum operator/(const auto& _num);
        void operator/=(const auto& _num);
        friend istream& operator>> (istream& is, BigNum& _num);
        friend ostream& operator<< (ostream& os, const BigNum& _num);
        BigNum FPoint(int _fpoint);
};
///Constructor
BigNum::BigNum() {
    _beforePoint.clear();
    _afterPoint.clear();
    _displayNumber.clear();
    _positive = true;
}
BigNum::BigNum(double _num) : BigNum::BigNum() {
    _displayNumber = to_string(_num);
    if(_displayNumber[0] == '-') _positive = false;
    ToNumber();
    FixNumber();
    ToDisplay();
}
BigNum::BigNum(long long _num) : BigNum::BigNum((double)_num) {}
BigNum::BigNum(int _num) : BigNum::BigNum((double)_num) {}
BigNum::BigNum(const BigNum& _num) {
    _beforePoint = _num._beforePoint;
    _afterPoint = _num._afterPoint;
    _displayNumber = _num._displayNumber;
    _positive = _num._positive;
}
BigNum::BigNum(string _error) : BigNum::BigNum() {
    _displayNumber = _error;
}
///Private Function
void BigNum::ToDisplay() {
    _displayNumber = "";
    if(!_positive) {
        _displayNumber += '-';
    }
    for(auto _x : _beforePoint) {
        _displayNumber += (char) (_x + '0');
    }
    if(!_afterPoint.empty()) {
        _displayNumber += '.';
        for(auto _x : _afterPoint) {
            _displayNumber += (char) (_x + '0');
        }
    }
}
void BigNum::FixDisplay() {
    string _newDisplay = "";
    bool havePoint = false;
    bool haveNum =  false;
    for(auto _x : _displayNumber) {
        if((_x == '-') && !haveNum) {
            if(_x == '-') {
                _positive = !_positive;
            }
        }
        if((_x == '.' || _x == ',') && haveNum && !havePoint) {
            _newDisplay += '.';
            havePoint = true;
        }
        if(_x >= '0' && _x <= '9') {
            _newDisplay += _x;
            haveNum = true;
        }
    }
    if(!_positive) _newDisplay = '-' + _newDisplay;
    _displayNumber = _newDisplay;
}
void BigNum::ToNumber() {
    _beforePoint.clear();
    _afterPoint.clear();
    bool havePoint = false;
    for(auto _x : _displayNumber) {
        if(_x == '.') {
            havePoint = true;
        }
        if(_x >= '0' && _x <= '9') {
            if(havePoint) _afterPoint.push_back((int)(_x - '0'));
            else _beforePoint.push_back((int)(_x - '0'));
        }
    }
}
void BigNum::FixNumber() {
    long long _mem = 0;
    for(int i = _afterPoint.size() - 1; i >= 0; --i) {
        _afterPoint[i] += _mem;
        if(_afterPoint[i] < 0) { _afterPoint[i] += 10; _mem = -1; }
        else { _mem = _afterPoint[i] / 10; _afterPoint[i] %= 10; }
    }
    while(!_afterPoint.empty() && (_afterPoint.back() == 0 || _afterPoint.size() > 2000)) _afterPoint.pop_back();
    reverse(_beforePoint.begin(), _beforePoint.end());
    for(int i = 0; i < _beforePoint.size(); ++i) {
        _beforePoint[i] += _mem;
        if(_beforePoint[i] < 0) { _beforePoint[i] += 10; _mem = -1; }
        else { _mem = _beforePoint[i] / 10; _beforePoint[i] %= 10; }
    }
    while(_mem > 0) {
        _beforePoint.push_back(_mem % 10);
        _mem /= 10;
    }
    while(_beforePoint.size() > 1 && _beforePoint.back() == 0) _beforePoint.pop_back();
    reverse(_beforePoint.begin(), _beforePoint.end());
    if(_beforePoint.size() == 1 && _afterPoint.empty() && _beforePoint[0] == 0) _positive = true;
}
void BigNum::FillZeros(BigNum& _num) {
    while(_afterPoint.size() < _num._afterPoint.size()) { _afterPoint.push_back(0); }
    while(_afterPoint.size() > _num._afterPoint.size()) { _num._afterPoint.push_back(0); }
    reverse(_beforePoint.begin(), _beforePoint.end());
    reverse(_num._beforePoint.begin(), _num._beforePoint.end());
    while(_beforePoint.size() < _num._beforePoint.size()) { _beforePoint.push_back(0); }
    while(_beforePoint.size() > _num._beforePoint.size()) { _num._beforePoint.push_back(0); }
    reverse(_beforePoint.begin(), _beforePoint.end());
    reverse(_num._beforePoint.begin(), _num._beforePoint.end());
}
BigNum BigNum::Inverse() {
    BigNum _fnum = 1;
    BigNum _snum = *this;
    _snum._positive = true;
    BigNum _res = 0;
    BigNum _count = 1;
    while(_snum <= 1) {
        _count *= 10;
        _snum *= 10;
    }
    while(_fnum != 0 && _res._afterPoint.size() <= 2000) {
        _fnum *= 10;
        for(int i = 9; i >= 0; --i) {
            if(_snum * i <= _fnum) {
                _res._afterPoint.push_back(i);
                _fnum -= (_snum * i);
                break;
            }
        }
    }
    _res *= _count;
    _res._positive = (*this)._positive;
    _res.ToDisplay();
    return _res;
}
///Operator
///Operator I/O
istream& operator>> (istream& is, BigNum& _num) {
    is >> _num._displayNumber;
    _num.FixDisplay();
    _num.ToNumber();
    _num.FixNumber();
    _num.ToDisplay();
    return is;
}
ostream& operator<< (ostream& os, const BigNum& _num) {
    os << _num._displayNumber;
    return os;
}
///Operator Compare
bool BigNum::operator==(const auto& _num) {
    BigNum _rep = _num;
    if(_positive != _rep._positive) return false;
    if(_beforePoint != _rep._beforePoint) return false;
    if(_afterPoint != _rep._afterPoint) return false;
    if(_displayNumber != _rep._displayNumber) return false;
    return true;
}
bool BigNum::operator!=(const auto& _num) {
    return !(*this == _num);
}
bool BigNum::operator>(const auto& _num) {
    if(*this == _num) return false;
    BigNum _rep = _num;
    if(_positive && !_rep._positive) return true;
    if(!_positive && _rep._positive) return false;
    if(_positive && _rep._positive) {
        if(_beforePoint.size() > _rep._beforePoint.size()) return true;
        if(_beforePoint.size() < _rep._beforePoint.size()) return false;
        for(int i = 0; i < _beforePoint.size(); ++i) {
            if(_beforePoint[i] > _rep._beforePoint[i]) return true;
            if(_beforePoint[i] < _rep._beforePoint[i]) return false;
        }
        for(int i = 0; i < min(_afterPoint.size(), _rep._afterPoint.size()); ++i) {
            if(_afterPoint[i] > _rep._afterPoint[i]) return true;
            if(_afterPoint[i] < _rep._afterPoint[i]) return false;
        }
        if(_afterPoint.size() > _rep._afterPoint.size()) return true;
        else return false;
    }
    return (-_rep > -(*this));
}
bool BigNum::operator>=(const auto& _num) {
    if(*this > _num || *this == _num) return true;
    return false;
}
bool BigNum::operator<(const auto& _num) {
    return !(*this >= _num);
}
bool BigNum::operator<=(const auto& _num) {
    return !(*this > _num);
}
///Operator Calculator
BigNum BigNum::operator-() {
    BigNum _rep = *this;
    _rep._positive = !_rep._positive;
    _rep.FixNumber();
    _rep.ToDisplay();
    return _rep;
}
BigNum BigNum::operator+(const auto& _num) {
    BigNum _fnum = *this;
    BigNum _snum = _num;
    if(_fnum._positive && _snum._positive) {
        _fnum._positive = true;
        _fnum.FillZeros(_snum);
        for(int i = 0; i < _fnum._beforePoint.size(); ++i) {
            _fnum._beforePoint[i] += _snum._beforePoint[i];
        }
        for(int i = 0; i < _fnum._afterPoint.size(); ++i) {
            _fnum._afterPoint[i] += _snum._afterPoint[i];
        }
        _fnum.FixNumber();
        _fnum.ToDisplay();
        return _fnum;
    }
    if(_snum._positive) {
        return _snum - (-_fnum);
    }
    return _fnum - (-_snum);
}
void BigNum::operator+=(const auto& _num) {
    *this = *this + _num;
}
BigNum BigNum::operator-(const auto& _num) {
    BigNum _fnum = *this;
    BigNum _snum = _num;
    if(_fnum._positive && _snum._positive) {
        if(_fnum >= _snum) {
            _fnum._positive = true;
            _fnum.FillZeros(_snum);
            for(int i = 0; i < _fnum._beforePoint.size(); ++i) {
                _fnum._beforePoint[i] -= _snum._beforePoint[i];
            }
            for(int i = 0; i < _fnum._afterPoint.size(); ++i) {
                _fnum._afterPoint[i] -= _snum._afterPoint[i];
            }
            _fnum.FixNumber();
            _fnum.ToDisplay();
            return _fnum;
        }
        else return -(_snum - _fnum);
    }
    if(!_fnum._positive && !_snum._positive) {
        return (-_snum)-(-_fnum);
    }
    if(!_fnum._positive && _snum._positive) {
        return -(-_fnum + _snum);
    }
    return _fnum + (-_snum);
}
void BigNum::operator-=(const auto& _num) {
    *this = *this - _num;
}
BigNum BigNum::operator*(const auto& _num) {
    BigNum _fnum = *this;
    BigNum _snum = _num;
    BigNum _res;
    _res._positive = (_fnum._positive == _snum._positive) ? true : false;
    _res._afterPoint.assign(_fnum._afterPoint.size() + _snum._afterPoint.size(), 0);
    _res._beforePoint.assign(_fnum._beforePoint.size() + _snum._beforePoint.size() - 1, 0);
    reverse(_fnum._beforePoint.begin(), _fnum._beforePoint.end());
    reverse(_snum._beforePoint.begin(), _snum._beforePoint.end());
    reverse(_fnum._afterPoint.begin(), _fnum._afterPoint.end());
    reverse(_snum._afterPoint.begin(), _snum._afterPoint.end());
    for(int i = 0; i < _fnum._beforePoint.size(); ++i) {
        for(int j = 0; j < _snum._beforePoint.size(); ++j) {
            _res._beforePoint[i + j] += (_fnum._beforePoint[i] * _snum._beforePoint[j]);
        }
        for(int j = 0; j < _snum._afterPoint.size(); ++j) {
            if(i + j + _fnum._afterPoint.size() < _res._afterPoint.size()) {
                _res._afterPoint[i + j + _fnum._afterPoint.size()] += (_fnum._beforePoint[i] * _snum._afterPoint[j]);
            }
            else {
                _res._beforePoint[i + j + _fnum._afterPoint.size() - _res._afterPoint.size()] += (_fnum._beforePoint[i] * _snum._afterPoint[j]);
            }
        }
    }
    for(int i = 0; i < _fnum._afterPoint.size(); ++i) {
        for(int j = 0; j < _snum._beforePoint.size(); ++j) {
            if(i + j + _snum._afterPoint.size() < _res._afterPoint.size()) {
                _res._afterPoint[i + j + _snum._afterPoint.size()] += (_fnum._afterPoint[i] * _snum._beforePoint[j]);
            }
            else {
                _res._beforePoint[i + j + _snum._afterPoint.size() - _res._afterPoint.size()] += (_fnum._afterPoint[i] * _snum._beforePoint[j]);
            }
        }
        for(int j = 0; j < _snum._afterPoint.size(); ++j) {
            _res._afterPoint[i + j] += (_fnum._afterPoint[i] * _snum._afterPoint[j]);
        }
    }
    reverse(_res._beforePoint.begin(), _res._beforePoint.end());
    reverse(_res._afterPoint.begin(), _res._afterPoint.end());
    _res.FixNumber();
    _res.ToDisplay();
    return _res;
}
void BigNum::operator*=(const auto& _num) {
    *this = *this * _num;
}
BigNum BigNum::operator/(const auto& _num) {
    BigNum _res;
    BigNum _rep = _num;
    if(_rep == 0) {
        _res = (_positive == _rep._positive) ? (string)"Inf" : (string)"-Inf";
        return _res;
    }
    _res = (*this)*_rep.Inverse();
    return _res.FPoint(1000);
}
///Public Function
BigNum BigNum::FPoint(int _fpoint) {
    BigNum _rep = *this;
    if(_rep._afterPoint.size() <= _fpoint) return _rep;
    _rep._afterPoint.resize(_fpoint + 1);
    int _mem = (_rep._afterPoint.back() >= 5) ? 1 : 0;
    _rep._afterPoint.pop_back();
    _rep._afterPoint.back() += _mem;
    _rep.FixNumber();
    _rep.ToDisplay();
    return _rep;
}
int main() {
    BigNum x, y;
    cin >> x >> y;
    cout << (x/y).FPoint(2);
}
