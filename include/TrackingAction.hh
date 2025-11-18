// ============================================================================
// File: include/TrackingAction.hh
// ============================================================================
#ifndef TRACKINGACTION_HH
#define TRACKINGACTION_HH

#include <G4UserTrackingAction.hh>
#include <globals.hh>

class RunAction;

class TrackingAction : public G4UserTrackingAction {
public:
	explicit TrackingAction(RunAction* runAction);
	~TrackingAction() override = default;

	void PreUserTrackingAction(const G4Track* track) override;

private:
	RunAction* fRunAction; // non-owning
};

#endif // TRACKINGACTION_HH
