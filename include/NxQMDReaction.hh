#ifndef NxQMDReaction_hh
#define NxQMDReaction_hh

#include "NxQMDSystem.hh"
#include "NxQMDCollision.hh"
#include "NxQMDMeanField.hh"
#include "NxQMDParticipant.hh"

#include "G4IonsShenCrossSection.hh"
//#include "G4GeneralSpaceNNCrossSection.hh"
#include "G4PiNuclearCrossSection.hh"

#include "G4HadronicInteraction.hh"

#include "G4Evaporation.hh"
#include "G4ExcitationHandler.hh"
//#include "G4PreCompoundModel.hh"

class NxQMDReaction : public G4HadronicInteraction
{
  public:
    NxQMDReaction();
    ~NxQMDReaction();

    std::vector< NxQMDSystem* > GetFinalStates(); 

    G4HadFinalState *ApplyYourself( const G4HadProjectile &aTrack, G4Nucleus & targetNucleus );

    G4ExcitationHandler* GetExcitationHandler(){ return excitationHandler; };

    void UnUseGEM(){ gem = false; setEvaporationCh(); };
    void UseFRAG(){ frag = true; };

    void SetTMAX( G4int i ){ maxTime = i; };
    void SetDT( G4double t ){ deltaT = t; };
    void SetEF( G4double x ){ envelopF = x; };

    virtual void ModelDescription(std::ostream& outFile) const;

    virtual G4bool IsApplicable(const G4HadProjectile &, G4Nucleus &);

  private:
    //copy is unexpeced
    void operator =( const NxQMDReaction& ){};
    NxQMDReaction( const NxQMDReaction& ):G4HadronicInteraction("QMDModel"){};

    void setEvaporationCh();
    void setHighEnergyModel();

    NxQMDMeanField* meanField;

    NxQMDCollision* collision;

    void doCollision();
    std::vector< NxQMDSystem* > doClusterJudgment();

    // Statistical Decay Models
    void SDM( G4int iz, G4int in, G4int jang, G4double ex, G4LorentzVector p, G4HadFinalState & );
    void SDMFISS( G4int iz1, G4int in1, G4double ex1, G4int j1, G4int &iz2, G4int &in2, G4double &ex2, G4int &j2, G4int &iz3, G4int &in3, G4double &ex3, G4int &j3, G4double &barf, G4double &saf );
    void SDMFISW( G4int iz1, G4int in1, G4double ex1, G4int j1, G4double &tcn, G4double &tcsf, G4double &barf, G4double &saf );
    void SDMWID0( G4int iz1, G4int in1, G4double ex1, G4int j1, G4int &iz2, G4int &in2, G4double &ex2, G4int &j2, G4int &iz3, G4int &in3, G4double &ex3, G4int &j3, G4double &totwid, G4double *pwid, G4int ifdec, G4int &ipdec );
    // Statistical Decay Models

    NxQMDSystem* system;
    G4double deltaT;
    G4int maxTime;
    G4double envelopF;

    G4Evaporation* evaporation;
    G4ExcitationHandler* excitationHandler;

    //      G4VPreCompoundModel* preco; 


    //                            b        pd_proj                pd_targ                  z_p     a_p     z_t     a_t      plab       elab
    //      G4double offSetOfCollision( G4double , G4ParticleDefinition* , G4ParticleDefinition* , G4int , G4int , G4int , G4int , G4double , G4double  );  
    //                           b          pd_proj                 pd_targ                 plab       elab      bmax boostToCM
    void calcOffSetOfCollision( G4double , const G4ParticleDefinition* , const G4ParticleDefinition* , G4double , G4double , G4double , G4ThreeVector );
    G4double coulomb_collision_gamma_proj;
    G4double coulomb_collision_rx_proj;
    G4double coulomb_collision_rz_proj;
    G4double coulomb_collision_px_proj;
    G4double coulomb_collision_pz_proj;

    G4double coulomb_collision_gamma_targ;
    G4double coulomb_collision_rx_targ;
    G4double coulomb_collision_rz_targ;
    G4double coulomb_collision_px_targ;
    G4double coulomb_collision_pz_targ;

    G4double gampr, gamta;// gamma of projectile and target
    G4double ElabJQMD;

    //090331
    G4IonsShenCrossSection* shenXS;
    //G4GeneralSpaceNNCrossSection* genspaXS;

    G4PiNuclearCrossSection* piNucXS;

    G4bool gem;
    G4bool frag;

    G4double izip[7] = { -99, 0, 1, 1, 1, 2, 2 };
    G4double inip[7] = { -99, 1, 0, 1, 2, 1, 2 };
    G4double jip[7] = { -99, 0, 0, 1, 0, 0, 0 };
    G4double sepc[7][2] = {
      { -99, -99 },
      { 1.0000, 1.00000 }, 
      { 8.0606, 0.50836 },
      { 7.9869, 0.62142 },
      { 7.9869, 0.62142 },
      { 6.4355, 0.78601 },
      { 6.1333, 0.88761 },
    };

    G4double qvalp[7]={0}; G4double eclbp[7]={0};
    G4double smi[7]={0}; G4double ey1[7]={0}; G4double s[7]={0};

    G4double nzz0[41] = { -1, 0, 1, 1, 1, 2, 1, 2, 3, 1, 2, 3, 4, 2, 3, 5, 4, 2, 3, 5, 4, 6, 2, 3, 5, 4, 6, 3, 5, 4, 6, 3, 7, 5, 4, 6, 3, 7, 5, 4, 6 };  
    G4double nnn0[41] = { -1, 1, 0, 1, 2, 1, 3, 2, 1, 4, 3, 2, 2, 4, 3, 2, 3, 5, 4, 3, 4, 2, 6, 5, 4, 5, 3, 6, 5, 6, 4, 7, 3, 6, 7, 5, 8, 4, 7, 8, 6 };

};

#endif
