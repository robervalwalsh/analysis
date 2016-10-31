#include <cmath>
#include "Riostream.h"
#include "TMath.h"
#include "Analysis/BackgroundModel/interface/RooQuadGausExp.h"


using namespace analysis::backgroundmodel;


ClassImp(RooQuadGausExp)


RooQuadGausExp::RooQuadGausExp(const char *name, const char *title,
                  RooAbsReal& x,
                  RooAbsReal& mean,
                  RooAbsReal& sigmaL1,
				  RooAbsReal& sigmaL2,
				  RooAbsReal& sigmaR1,
				  RooAbsReal& sigmaR2,
				  RooAbsReal& tail_shift,
				  RooAbsReal& tail_sigma,
				  RooAbsReal& norm_g1,
				  RooAbsReal& norm_g2) :
  RooAbsPdf(name, title),
  x_("x", "x", this, x),
  mean_("mean", "mean", this, mean),
  sigmaL1_("sigmaL1", "sigmaL1", this, sigmaL1),
  sigmaL2_("sigmaL2", "sigmaL2", this, sigmaL2),
  sigmaR1_("sigmaR1", "sigmaR1", this, sigmaR1),
  sigmaR2_("sigmaR2", "sigmaR2", this, sigmaR2),
  tail_shift_("tail_shift", "tail_shift", this, tail_shift),
  tail_sigma_("tail_sigma", "tail_sigma", this, tail_sigma),
  norm_g1_("norm_g1", "norm_g1", this, norm_g1),
  norm_g2_("norm_g2", "norm_g2", this, norm_g2)
  {}


RooQuadGausExp::RooQuadGausExp(const RooQuadGausExp& other, const char* name) :
  RooAbsPdf(other, name),
  x_("x", this, other.x_),
  mean_("mean", this, other.mean_),
  sigmaL1_("sigmaL1", this, other.sigmaL1_),
  sigmaL2_("sigmaL2", this, other.sigmaL2_),
  sigmaR1_("sigmaR1", this, other.sigmaR1_),
  sigmaR2_("sigmaR2", this, other.sigmaR2_),
  tail_shift_("tail_shift", this, other.tail_shift_),
  tail_sigma_("tail_sigma", this, other.tail_sigma_),
  norm_g1_("norm_g1", this, other.norm_g1_),
  norm_g2_("norm_g2", this, other.norm_g2_) {
}


TObject* RooQuadGausExp::clone(const char* newname) const {
  return new RooQuadGausExp(*this, newname);
}


double RooQuadGausExp::evaluate() const {

	double aL = (x_-mean_)/sigmaL1_;
	double bL = (x_-mean_)/sigmaL2_;
	double aR = (x_-mean_)/sigmaR1_;
	double bR = (x_-mean_)/sigmaR2_;

	double aexp = (x_-tail_shift_)/tail_sigma_;

	double cR  = (tail_shift_-mean_)/sigmaR1_;
	double dR  = (tail_shift_-mean_)/sigmaR2_;

	double gL = norm_g1_*TMath::Exp(-0.5*aL*aL)+(1-norm_g1_)*TMath::Exp(-0.5*bL*bL);
	double gR = norm_g2_*TMath::Exp(-0.5*aR*aR)+(1-norm_g2_)*TMath::Exp(-0.5*bR*bR);

	double w = norm_g2_*TMath::Exp(-0.5*cR*cR)+(1-norm_g2_)*TMath::Exp(-0.5*dR*dR);

	double resultL = gL;
	double resultR = gR;
	double norm    = w;
	double exp = norm*TMath::Exp(-aexp);

	double result = resultL;
	if (x_>mean_)
		result = resultR;
	if (x_>tail_shift_)
		result = exp;

  return result;
}



