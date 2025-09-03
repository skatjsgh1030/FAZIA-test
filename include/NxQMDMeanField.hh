#ifndef NxQMDMeanField_hh
#define NxQMDMeanField_hh

#include "NxQMDSystem.hh"
#include "NxQMDNucleus.hh"

class NxQMDMeanField 
{
   public:
      NxQMDMeanField();
      ~NxQMDMeanField();

      void SetSystem ( NxQMDSystem* aSystem );
      void SetNucleus ( NxQMDNucleus* aSystem );
      NxQMDSystem* GetSystem () {return system; };

      void SetNuc( G4bool is ) { NucNucColl = is; }

      void Cal2BodyQuantities(); 
      void Cal2BodyQuantities( G4int ); 

      void CalGraduate(); 

      G4bool IsPauliBlocked( G4int ); 

      G4double GetTotalPotential(); 
      G4double GetPotential( G4int );

      void DoPropagation( G4double );

      std::vector< NxQMDNucleus* > DoClusterJudgment();

      G4double GetRR2( G4int i , G4int j ) { return rr2[i][j]; };

      G4double GetRHA( G4int i , G4int j ) { return rha[i][j]; };
      G4double GetRHE( G4int i , G4int j ) { return rhe[i][j]; };
      G4ThreeVector GetFFr( G4int i ) { return ffr[i]; };
      G4ThreeVector GetFFp( G4int i ) { return ffp[i]; };

      std::vector< G4double > GetLocalDensity();  
      std::vector< G4double > GetDepthOfPotential();  

      void Update();

   private:
      G4double ERF( G4double );

      G4double calPauliBlockingFactor( G4int ); 

      NxQMDSystem* system; 

      G4double rclds;

      G4double hbc , rho0;
      G4double epsx , epscl; 

      G4double cpc;

      //G4int icoul, irelcr;
      G4int irelcr;
      G4double gamm, c0, c3, cs, cl, wl; 
      //G4double c0w, c3w, clw, c0sw;
      G4double c0w, clw, c0sw;

      G4double c0g,c3g,csg,pag; 

      G4double cpw,cph;
       
      // 2 Body Quantities 
      std::vector < std::vector < G4double > > rr2;    
      std::vector < std::vector < G4double > > pp2;    
      std::vector < std::vector < G4double > > rbij;    

      // Gauss 
      std::vector < std::vector < G4double > > rha;    

      // Coulomb
      std::vector < std::vector < G4double > > rhe;    
      std::vector < std::vector < G4double > > rhc;    
                                         
      std::vector < G4ThreeVector > ffr;    
      std::vector < G4ThreeVector > ffp;    
      std::vector < G4double > rh3d;    

      G4bool NucNucColl = false;
       
};

#endif
