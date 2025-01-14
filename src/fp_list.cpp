#include "fp_list.h"

void MyList::getMin(double *min_, int *origine_, double *mean_)
{
    *min_ = std::numeric_limits<double>::max();
    for (auto Current = Listes.begin(); Current != Listes.end(); ++Current) {
        auto CurrentCFunc = Current->get();
        double tmp = CurrentCFunc->min();
        if (*min_ > tmp) {
            *min_ = tmp;
            *origine_ = CurrentCFunc->origine;
            *mean_ = CurrentCFunc->mean();
        }
    }
}
void MyList::add(double X_, double lthrs_, double rthrs_, double lslope, double rslope)
{
    double lBorder = X_ - lthrs_;
    double rBorder = X_ + rthrs_;

    double a0 = X_ * X_;
    double a1 = -2 * X_;
    double a2 = 1;

    double la1 = lslope;
    double la0 = lthrs_ * lthrs_ - la1 * lBorder;

    double ra1 = rslope;
    double ra0 = rthrs_ * rthrs_ - ra1 * rBorder;

    auto it = Listes.begin();
    auto CurFunc = it->get();
    // add left //
    bool aL = true;
    do {
        CurFunc = it->get();
        aL = CurFunc->addLeft(lBorder, la0, la1);
    } while (aL && (++it != Listes.end()));
    if (it == Listes.end()) it--;

    if ((CurFunc->rBorder != lBorder) && (CurFunc->lBorder < lBorder)) {
        auto cFunc = std::make_unique<CFunctPart>(CurFunc->a0, CurFunc->a1, CurFunc->a2, lBorder, CurFunc->rBorder,
                                                    CurFunc->origine);
        it = Listes.insert(++it, std::move(cFunc));
        CurFunc->a0 += la0;
        CurFunc->a1 += la1;
        CurFunc->rBorder = lBorder;
        CurFunc = it->get();
    }

    // add polynomial part (center part) //
    bool aC = true;
    do {
        CurFunc = it->get();
        aC = CurFunc->addCenter(rBorder, a0, a1, a2);
    } while (aC && (++it != Listes.end()));
    if (it == Listes.end()) it--;

    // splitRight if necessary //
    if (CurFunc->rBorder > rBorder) {
        auto cFunc = std::make_unique<CFunctPart>(CurFunc->a0, CurFunc->a1, CurFunc->a2, rBorder, CurFunc->rBorder,
                                                    CurFunc->origine);
        it = Listes.insert(++it, std::move(cFunc));
        CurFunc->a0 += a0;
        CurFunc->a0 += a1;
        CurFunc->a0 += a2;
        CurFunc->rBorder = rBorder;
    }
    // add right //
    while (++it != Listes.end()) {
        CurFunc = it->get();
        CurFunc->addRight(ra0, ra1);
    }
}

void MyList::compare(double cost_, int origine_)
{
    auto it = Listes.begin();
    auto CurFunc = it->get();
    while (it != Listes.end()) {
        CurFunc = it->get();
        CurFunc->updateRoots(cost_);
        if (CurFunc->racStatus == -1) {
            CurFunc->reset(cost_, origine_);
        }
        if (CurFunc->racStatus == 1) {
            // splitRight if necessary //
            if (CurFunc->rac1 > CurFunc->lBorder) {
                auto cFunc = std::make_unique<CFunctPart>(CurFunc->a0, CurFunc->a1, CurFunc->a2, CurFunc->rac1,
                                                    CurFunc->rBorder, CurFunc->rac1, CurFunc->rac2, CurFunc->origine);
                it = Listes.insert(++it, std::move(cFunc));
                CurFunc->reset(cost_, origine_);
                CurFunc->rBorder = it->get()->lBorder;
                CurFunc = it->get();
            }
            // splitRight if necessary //
            if (CurFunc->rac2 < CurFunc->rBorder) {
                auto cFunc = std::make_unique<CFunctPart>(cost_, 0, 0, CurFunc->rac2, CurFunc->rBorder, origine_);
                it = Listes.insert(++it, std::move(cFunc));
                CurFunc->rBorder = CurFunc->rac2;
                CurFunc->rac1 = 0.;
                CurFunc->rac2 = 0.;
                CurFunc->racStatus = 0;
                CurFunc = it->get();
            }
        }
        it++;
    }
}

void MyList::mergeSimilarElements()
{
    auto it = Listes.begin();
    auto CurFunc = it->get();
    while (it != Listes.end()) {
        CurFunc = it->get();
        it++;
        if ((it != Listes.end()) && (it->get()->origine == CurFunc->origine) &&
            (it->get()->a2 == CurFunc->a2) && (it->get()->a1 == CurFunc->a1) &&
            (it->get()->a0 == CurFunc->a0)) {
            CurFunc->rBorder = it->get()->rBorder;
            it = Listes.erase(it);
            it--;
        }
    }
}