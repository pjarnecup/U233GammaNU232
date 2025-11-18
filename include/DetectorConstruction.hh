// ============================================================================
// File: include/DetectorConstruction.hh
// ============================================================================
#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>
#include <globals.hh>

class G4LogicalVolume;
class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
	DetectorConstruction();
	~DetectorConstruction() override = default;

	G4VPhysicalVolume* Construct() override;
	inline G4LogicalVolume* GetTargetLV() const { return fTargetLV; }

private:
	void DefineMaterials();
	G4LogicalVolume* fTargetLV = nullptr; // why: TrackingAction uses its name to localize production
};

#endif // DETECTORCONSTRUCTION_HH