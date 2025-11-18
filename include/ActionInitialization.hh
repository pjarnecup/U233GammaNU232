// ============================================================================
// File: include/ActionInitialization.hh
// ============================================================================
#ifndef ACTIONINITIALIZATION_HH
#define ACTIONINITIALIZATION_HH

#include <G4VUserActionInitialization.hh>

class DetectorConstruction;

class ActionInitialization : public G4VUserActionInitialization {
public:
	explicit ActionInitialization(DetectorConstruction* det);
	~ActionInitialization() override = default;

	void BuildForMaster() const override;
	void Build() const override;

private:
	DetectorConstruction* fDet; // non-owning
};

#endif // ACTIONINITIALIZATION_HH