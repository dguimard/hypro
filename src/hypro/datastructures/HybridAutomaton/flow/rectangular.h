#pragma once
#include <carl/interval/Interval.h>
#include <vector>

namespace hypro {

template<typename Number>
class rectangularFlow {
public:
protected:
    std::vector<carl::Interval<Number>> mFlowIntervals;
public:
    rectangularFlow() = default;
    rectangularFlow(const rectangularFlow<Number>& in) = default;
    rectangularFlow(const std::vector<carl::Interval<Number>>& intervals) : mFlowIntervals(intervals) {}
    virtual ~rectangularFlow() {}

    static DynamicType type() { return DynamicType::rectangular; }

    void setFlowIntervals(const std::vector<carl::Interval<Number>>& in ) { mFlowIntervals = in; }
    void setFlowIntervalForDimension(const carl::Interval<Number>& intv, std::size_t dim) {
        assert(this->size() > dim);
        mFlowIntervals[dim] = intv;
    }
    void addFlowInterval(const carl::Interval<Number>& intv) { mFlowIntervals.push_back(intv); }

    const std::vector<carl::Interval<Number>>& getFlowIntervals() const { return mFlowIntervals; }
    const carl::Interval<Number>& getFlowIntervalForDimension(std::size_t dim) const { assert(this->dimension() > dim); return mFlowIntervals[dim]; }

    std::size_t dimension() const { return mFlowIntervals.size(); }
    std::size_t size() const { return mFlowIntervals.size(); }

    bool isTimed() const {
        return std::count(mFlowIntervals.begin(), mFlowIntervals.end(), carl::Interval<Number>(1)) == long(mFlowIntervals.size());
    }

    bool isDiscrete() const {
        return std::count(mFlowIntervals.begin(), mFlowIntervals.end(), carl::Interval<Number>(0)) == long(mFlowIntervals.size());
    }

    friend bool operator==(const rectangularFlow<Number>& lhs, const rectangularFlow<Number>& rhs) {
        if(lhs.getFlowIntervals().size() != rhs.getFlowIntervals().size()) {
            return false;
        }

        for(std::size_t i = 0; i < lhs.getFlowIntervals().size(); ++i) {
            if(lhs.getFlowIntervalForDimension(i) != rhs.getFlowIntervalForDimension(i)) {
                return false;
            }
        }
        return true;
    }

    friend bool operator!=(const rectangularFlow<Number>& lhs, const rectangularFlow<Number>& rhs) {
        return !(lhs == rhs);
    }

    friend ostream& operator<<(ostream& out, const rectangularFlow<Number>& in) {
        for(const auto& i : in.getFlowIntervals()) {
            out << i << ", ";
        }
        return out;
    }
};

} // hypro

namespace std {

    template<typename Number>
    struct hash<hypro::rectangularFlow<Number>> {
        std::size_t operator()(const hypro::rectangularFlow<Number>& f) const
        {
            std::size_t seed = 0;
            for(const auto& i : f.getFlowIntervals()) {

                carl::hash_add(seed, std::hash<carl::Interval<Number>>()(i));
            }
            return seed;
        }
    };

}  // namespace std
