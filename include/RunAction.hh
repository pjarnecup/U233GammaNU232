// ============================================================================
// File: include/RunAction.hh
// ============================================================================
#ifndef RUNACTION_HH
#define RUNACTION_HH

#include <G4UserRunAction.hh>
#include <G4Accumulable.hh>
#include <G4AccumulableManager.hh>
#include <G4GenericMessenger.hh>
#include <globals.hh>
#include <memory>

class G4Run;

class RunAction : public G4UserRunAction {
public:
	RunAction();
	~RunAction() override = default;

	void BeginOfRunAction(const G4Run*) override;
	void EndOfRunAction(const G4Run*) override;

	// thread-safe increments
	void IncrementU232();
	void IncrementNPrimaries();

	// accessors (used in tracking)
	inline const G4String& TargetLVName() const { return fTargetLVName; }
	void SetTargetLVName(const G4String& name) { fTargetLVName = name; }

private:
	// accumulables
	G4Accumulable<G4long> fCountU232;
	G4Accumulable<G4long> fNPrimaries;

	// user-configurable parameters
	G4double fBeamCurrentA; // default 0.25 mA
	G4double fIrradTimeS;   // default 3600 s
	std::unique_ptr<G4GenericMessenger> fMessenger;

	// cached names
	G4String fTargetLVName;

	// helpers
	void SetupMessenger();
};

#endif // RUNACTION_HH