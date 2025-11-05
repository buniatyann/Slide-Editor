#include "model/SlideRepository.hpp"
#include "serialization/JsonSerializer.hpp"
#include "view/cli/CliView.hpp"
#include "controller/CommandController.hpp"
#include "io/InputStream.hpp"
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) { 
    using namespace slideEditor;

    std::ignore = argc;
    std::ignore = argv;
    
    // Create core components
    auto repository = std::make_unique<model::SlideRepository>();
    auto serializer = std::make_unique<serialization::JsonSerializer>();
    auto view = std::make_unique<view::CliView>(std::cout);
    auto inputStream = std::make_unique<io::InputStream>(std::cin);
    
    controller::CommandController controller(
        repository.get(),
        serializer.get(),
        view.get(),
        inputStream.get()
    );
    
    try {
        controller.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}