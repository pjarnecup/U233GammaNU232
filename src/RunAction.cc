// ============================================================================
// File: src/RunAction.cc
// ============================================================================
#include "RunAction.hh"

#include <G4Run.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4LogicalVolume.hh>
#include <G4Tubs.hh>
#include <G4Material.hh>
#include <G4Element.hh>
#include <G4Isotope.hh>
#include <G4PhysicalConstants.hh>   // Avogadro
#include <iomanip>

namespace {
    constexpr G4double kElectronChargeC = 1.602176634e-19; // why: convert A->e-/s exactly
}

RunAction::RunAction()
    : fCountU232(0), fNPrimaries(0),
    fBeamCurrentA(0.0005), fIrradTimeS(3600.0 * 24 * 365 * 2.5) { //2.5 years
    auto* acc = G4AccumulableManager::Instance();   // <-- pointer
    acc->RegisterAccumulable(fCountU232);
    acc->RegisterAccumulable(fNPrimaries);
    SetupMessenger();
}

void RunAction::SetupMessenger() {
    fMessenger = std::make_unique<G4GenericMessenger>(this, "/run/", "Run controls");
    fMessenger->DeclarePropertyWithUnit("beamCurrent", "A", fBeamCurrentA,
        "Beam current in amperes (default 0.0005 A).");
    fMessenger->DeclarePropertyWithUnit("irradTime", "s", fIrradTimeS,
        "Irradiation time in seconds for projection.");
}

void RunAction::BeginOfRunAction(const G4Run*) {
    auto* acc = G4AccumulableManager::Instance();
    acc->Reset();
    if (fTargetLVName.empty()) fTargetLVName = "TargetLV";
    G4cout << "\n=== Begin Of Run ===\n"
        << " beamCurrent = " << fBeamCurrentA << " A"
        << ", irradTime = " << fIrradTimeS << " s\n"
        << " targetLV = " << fTargetLVName
        << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* run) {
    auto* acc = G4AccumulableManager::Instance();
    acc->Merge();

    const G4long nU232 = fCountU232.GetValue();
    const G4long nPrim = fNPrimaries.GetValue();

    if (run->GetNumberOfEvent() == 0 || nPrim == 0) {
        G4cout << "\n=== End Of Run ===\nNo events or primaries. Nothing to report.\n" << G4endl;
        return;
    }

    const double perPrimary = static_cast<double>(nU232) / static_cast<double>(nPrim);
    const double electronsPerSec = fBeamCurrentA / kElectronChargeC;
    const double yieldAtomsPerSec = perPrimary * electronsPerSec;
    const double projectedAtoms = yieldAtomsPerSec * fIrradTimeS;
	const double activity = projectedAtoms * (std::log(2) / ((68.9 * year)/s)); // U-232 half-life
	const double activityInCi = activity / (3.7e10); // 1 Ci = 3.7e10 decays/s

    G4cout << std::setprecision(6) << std::scientific;
    G4cout << "\n=== End Of Run Summary ===\n"
        << " Events simulated : " << run->GetNumberOfEvent() << "\n"
        << " Primaries (e-)   : " << nPrim << "\n"
        << " U-232 produced   : " << nU232 << "\n"
        << " P(prod | primary): " << perPrimary << "  [atoms / e-]\n"
        << " Beam current     : " << fBeamCurrentA << " A  -> "
        << electronsPerSec << " e-/s\n"
        << " Yield            : " << yieldAtomsPerSec << " atoms/s\n"
        << " Irrad time       : " << fIrradTimeS << " s\n"
        << " Projected atoms  : " << projectedAtoms << " atoms\n"
		<< " Projected activity: " << activityInCi << " Ci of U-232\n"
        << G4endl;
}

void RunAction::IncrementU232() { fCountU232 += 1; }
void RunAction::IncrementNPrimaries() { fNPrimaries += 1; }
