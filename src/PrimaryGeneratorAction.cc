// ============================================================================
// File: src/PrimaryGeneratorAction.cc
// ============================================================================
#include "PrimaryGeneratorAction.hh"

#include <G4ParticleGun.hh>
#include <G4Gamma.hh>
#include <G4ParticleTable.hh>
#include <G4Electron.hh>
#include <G4SystemOfUnits.hh>
#include <G4GenericMessenger.hh>
#include <G4Event.hh>
#include <Randomize.hh>
#include <cmath>

PrimaryGeneratorAction::PrimaryGeneratorAction()
	: fBeamRadius(2.5 * mm) {
	fGun = new G4ParticleGun(1);
	fGun->SetParticleDefinition(G4Electron::Definition());
	fGun->SetParticleEnergy(40 * MeV);
	// Start 1 cm upstream from target center
	fGun->SetParticlePosition(G4ThreeVector(0., 0., -1.0 * cm));
	fGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
	delete fGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
	// Uniform disk sampling (avoid center bias)
	const G4double u = G4UniformRand();
	const G4double v = G4UniformRand();
	const G4double rho = fBeamRadius * std::sqrt(u);
	const G4double phi = 2. * CLHEP::pi * v;
	const G4double x = rho * std::cos(phi);
	const G4double y = rho * std::sin(phi);

	auto pos = fGun->GetParticlePosition();
	pos.setX(x); pos.setY(y);
	fGun->SetParticlePosition(pos);

	fGun->GeneratePrimaryVertex(event);
}
