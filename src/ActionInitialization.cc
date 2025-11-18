// ============================================================================
// File: src/ActionInitialization.cc
// ============================================================================
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "TrackingAction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction* det)
	: fDet(det) {
}

void ActionInitialization::BuildForMaster() const {
	auto* runAction = new RunAction();
	runAction->SetTargetLVName("TargetLV");
	SetUserAction(runAction);
}

void ActionInitialization::Build() const {
	auto* runAction = new RunAction();
	runAction->SetTargetLVName("TargetLV");
	SetUserAction(runAction);

	SetUserAction(new PrimaryGeneratorAction());
	SetUserAction(new TrackingAction(runAction));
}
