#ifndef KSCINInelasticCrossSection_HH
#define KSCINInelasticCrossSection_HH

#include "globals.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"

#include "G4VCrossSectionDataSet.hh"

#include "KSCINFunction.hh"


using namespace CLHEP;

class KSCINInelasticCrossSection : public G4VCrossSectionDataSet
{
  public:
    KSCINInelasticCrossSection();
    virtual ~KSCINInelasticCrossSection();

    virtual const G4Isotope* SelectIsotope(const G4Element* anElement, G4double , G4double );

    virtual G4bool IsIsoApplicable(const G4DynamicParticle* aDP, G4int Z, G4int A, const G4Element*, const G4Material*);
    virtual G4double GetIsoCrossSection(const G4DynamicParticle*, G4int Z, G4int A, const G4Isotope* iso = 0, const G4Element* elm = 0, const G4Material* mat = 0);

    virtual void CrossSectionDescription(std::ostream&) const;

  private:

    G4double GetCarbonElastic(G4double);
    G4double GetNGAMMAXS(G4double);
    G4double GetN2NXS(G4double);
    G4double GetN3HEXS(G4double);
    G4double GetNALPHAXS(G4double);
    G4double GetNDXS(G4double);
    G4double GetN3AXS(G4double);
    G4double GetNPXS(G4double);
    G4double GetNPNXS(G4double);
    G4double GetNTXS(G4double);

};
#endif
