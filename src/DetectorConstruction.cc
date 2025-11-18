// ============================================================================
// File: src/DetectorConstruction.cc
// ============================================================================
#include "DetectorConstruction.hh"

#include <G4Box.hh>
#include <G4Isotope.hh>
#include <G4Element.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4RotationMatrix.hh>
#include <G4SystemOfUnits.hh>
#include <G4Tubs.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>

DetectorConstruction::DetectorConstruction() = default;

void DetectorConstruction::DefineMaterials() {
	// U-233 metal (pure isotope)
	auto* U233 = new G4Isotope("U233", 92, 233, 233.039635 * g / mole);
	auto* elU = new G4Element("Uranium233", "U233", 1);
	elU->AddIsotope(U233, 100. * perCent);

	// Metallic uranium density ~18.95 g/cm3
	auto density = 18.95 * g / cm3;
	new G4Material("U233Metal", density, 1, kStateSolid);
	G4Material* U233Metal = G4Material::GetMaterial("U233Metal");
	U233Metal->AddElement(elU, 1);
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
	DefineMaterials();

	auto* nist = G4NistManager::Instance();
	G4Material* air = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* U233Metal = G4Material::GetMaterial("U233Metal");

	// World
	const G4double worldHalf = 10. * cm;
	auto* solidWorld = new G4Box("World", worldHalf, worldHalf, worldHalf);
	auto* logicWorld = new G4LogicalVolume(solidWorld, air, "WorldLV");
	auto* physWorld = new G4PVPlacement(nullptr, {}, logicWorld, "World", nullptr, false, 0, true);

	// Target: cylinder Ø1 cm × 1 cm, axis Z
	const G4double r = 0.5 * cm;
	const G4double hz = 0.5 * cm; // half-length
	auto* solidTarget = new G4Tubs("Target", 0., r, hz, 0. * deg, 360. * deg);
	fTargetLV = new G4LogicalVolume(solidTarget, U233Metal, "TargetLV");
	new G4PVPlacement(nullptr, {}, fTargetLV, "Target", logicWorld, false, 0, true);

	// Visuals
	logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
	auto* targetVis = new G4VisAttributes(G4Colour(0.6, 0.6, 0.2));
	targetVis->SetForceSolid(true);
	fTargetLV->SetVisAttributes(targetVis);

	return physWorld;
}
