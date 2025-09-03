#ifndef KSCINElasticCrossSection_HH
#define KSCINElasticCrossSection_HH

#include "globals.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"

#include "G4VCrossSectionDataSet.hh"

#include "stdio.h"
#include "stdlib.h"

using namespace std;

using namespace CLHEP;

class KSCINElasticCrossSection : public G4VCrossSectionDataSet
{
  public:
    KSCINElasticCrossSection();
    virtual ~KSCINElasticCrossSection();
    virtual G4bool IsElementApplicable(const G4DynamicParticle* aDP, G4int Z, const G4Material*);
    virtual G4bool IsIsoApplicable(const G4DynamicParticle* aDP, G4int Z, G4int A, const G4Element*, const G4Material*);
    virtual const G4Isotope* SelectIsotope(const G4Element* anElement, G4double , G4double );


    virtual G4double GetElementCrossSection(const G4DynamicParticle*, G4int Z, const G4Material*);
    virtual G4double GetIsoCrossSection(const G4DynamicParticle*, G4int Z, G4int A, const G4Isotope* iso = 0, const G4Element* elm = 0, const G4Material* mat = 0);

    virtual void CrossSectionDescription(std::ostream&) const;

  private:

};

#endif
