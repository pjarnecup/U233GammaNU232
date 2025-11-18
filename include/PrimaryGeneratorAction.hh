// ============================================================================
// File: include/PrimaryGeneratorAction.hh
// ============================================================================
#ifndef PRIMARYGENERATORACTION_HH
#define PRIMARYGENERATORACTION_HH

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ThreeVector.hh>
#include <globals.hh>

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
	PrimaryGeneratorAction();
	~PrimaryGeneratorAction() override;

	void GeneratePrimaries(G4Event* anEvent) override;

private:
	G4ParticleGun* fGun = nullptr;
	G4double fBeamRadius; // 2.5 mm

	void SetupMessenger(); //added for cs
};

#endif // PRIMARYGENERATORACTION_HH