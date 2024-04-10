#include <torch/torch.h>
#include <torch/script.h>
#include <iostream>

int main() {
    torch::jit::script::Module module;

    try {
      // Deserialize the ScriptModule from a file using torch::jit::load().
      module = torch::jit::load("../../traced_nn.pt");
    }
    catch (const c10::Error& e) {
      std::cerr << "error loading the model\n";
      return -1;
    }
}