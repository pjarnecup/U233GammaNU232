// ============================================================================
// File: src/main.cc
// ============================================================================
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include <G4RunManagerFactory.hh>
#include <G4UImanager.hh>
#include <G4UIExecutive.hh>
#include <G4VisExecutive.hh>
#include <G4PhysListFactory.hh>
#include <G4VModularPhysicsList.hh>
#include <G4EmExtraPhysics.hh>
#include <G4DecayPhysics.hh>
#include <G4SystemOfUnits.hh>
#include <G4Threading.hh>
#include "Randomize.hh" // G4Random / CLHEP::HepRandom

#include <chrono>
#include <cstdlib>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

static long MakeSeed() {
    if (const char* s = std::getenv("G4SEED")) {
        char* end = nullptr;
        long v = std::strtol(s, &end, 10);
        if (end && *end == '\0' && v > 0) return v;  // all digits, positive
    }
    auto now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count();
#ifdef _WIN32
    long pid = static_cast<long>(_getpid());
#else
    long pid = static_cast<long>(getpid());
#endif
    long seed = static_cast<long>((now_ns ^ (static_cast<long long>(pid) << 16)) & 0x7fffffffL);
    if (seed <= 0) seed = 1;
    return seed;
}

int main(int argc, char** argv) {
    G4UIExecutive* ui = (argc == 1) ? new G4UIExecutive(argc, argv) : nullptr;

    // RNG engine + seed (must be set before /run/initialize)
    G4Random::setTheEngine(new CLHEP::MTwistEngine());  // why: fast, MT-friendly
    const long seed = MakeSeed();
    G4Random::setTheSeed(seed);
    G4cout << "[RNG] Engine=MTwistEngine, master-seed=" << seed << G4endl;
    // Note: In MT mode, Geant4 derives distinct worker seeds from this master seed.


    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());

    auto* det = new DetectorConstruction();
    runManager->SetUserInitialization(det);

    G4PhysListFactory factory;
    G4VModularPhysicsList* phys = factory.GetReferencePhysList("FTFP_BERT");
    phys->RegisterPhysics(new G4EmExtraPhysics());
    phys->RegisterPhysics(new G4DecayPhysics());
    runManager->SetUserInitialization(phys);

    runManager->SetUserInitialization(new ActionInitialization(det));

    auto* vis = new G4VisExecutive("quiet");
    vis->Initialize();

    auto* UImanager = G4UImanager::GetUIpointer();

    if (ui) {
        UImanager->ApplyCommand("/control/execute macros/init_vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else {
        UImanager->ApplyCommand(G4String("/control/execute ") + argv[1]);
    }

    delete vis;
    delete runManager;
    return 0;
}