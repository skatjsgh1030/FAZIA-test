#include "PhysicsList.hh"

#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh" 

MyPhysicsList::MyPhysicsList()
{
    RegisterPhysics (new G4EmStandardPhysics()); // electromagnetic interaction
   // RegisterPhysics (new G4OpticalPhysics()); //optical photon                                                                                                                                                
    // Hadronic 상호작용 추가
    // 50 MeV 에너지에 적합한 Hadron Physics List
    RegisterPhysics (new G4HadronPhysicsFTFP_BERT());
}

MyPhysicsList::~MyPhysicsList()
{
}
