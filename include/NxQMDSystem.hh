#ifndef NxQMDSystem_hh
#define NxQMDSystem_hh

#include "NxQMDParticipant.hh"

class NxQMDSystem 
{
   public:
      NxQMDSystem();
      virtual ~NxQMDSystem();

      void SetParticipant( NxQMDParticipant* particle ) { participants.push_back ( particle ); };
      void SetSystem ( NxQMDSystem* , G4ThreeVector , G4ThreeVector );

      void SubtractSystem ( NxQMDSystem* );

      NxQMDParticipant* EraseParticipant( G4int i ) { NxQMDParticipant* particle =  participants[ i ]; participants.erase( std::find ( participants.begin() , participants.end() , participants[ i ] ) ) ; return particle; };
      void DeleteParticipant( G4int i ) { delete participants[ i ] ; participants.erase( std::find ( participants.begin() , participants.end() , participants[ i ] ) ); };
      void InsertParticipant( NxQMDParticipant* particle , G4int j );

      G4int GetTotalNumberOfParticipant() { return participants.size(); };

      NxQMDParticipant* GetParticipant( G4int i ) { return participants[i]; };

      void IncrementCollisionCounter() { numberOfCollision++; };
      G4int GetNOCollision() { return numberOfCollision; };

      void ShowParticipants();

      void Clear();

   protected:
      std::vector< NxQMDParticipant* > participants;

   private:
      G4int numberOfCollision;
};

#endif
