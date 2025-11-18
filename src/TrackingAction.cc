// ============================================================================
// File: src/TrackingAction.cc
// ============================================================================
#include "TrackingAction.hh"
#include "RunAction.hh"

#include <G4Track.hh>
#include <G4ParticleDefinition.hh>
#include <G4Ions.hh>
#include <G4VProcess.hh>
#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolume.hh>
#include <G4ParticleTypes.hh>

TrackingAction::TrackingAction(RunAction* runAction)
    : fRunAction(runAction) {
}

void TrackingAction::PreUserTrackingAction(const G4Track* track) {
    // Count primaries: now gamma
    if (track->GetParentID() == 0) {
        if (track->GetParticleDefinition() == G4Electron::Definition()) {
            fRunAction->IncrementNPrimaries();
        }
    }

    // Only interested in secondaries that are ions produced by photonuclear in TargetLV
    if (track->GetParentID() == 0) return; // skip primaries

    auto* ion = dynamic_cast<const G4Ions*>(track->GetParticleDefinition());
    if (!ion) return;

    const G4int Z = ion->GetAtomicNumber();
    const G4int A = ion->GetAtomicMass();

    if (Z != 92 || A != 232) return;

    const G4VProcess* creator = track->GetCreatorProcess();
    if (!creator) return;

    // Require photonuclear origin
    if (creator->GetProcessName() != "photonNuclear") return;

    // Limit to production inside target logical volume
    const G4VPhysicalVolume* pv = track->GetVolume();
    if (!pv) return;
    const G4LogicalVolume* lv = pv->GetLogicalVolume();
    if (!lv) return;
    if (lv->GetName() != fRunAction->TargetLVName()) return;

    fRunAction->IncrementU232();
}
