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
    
    // core components
    auto repository = std::make_shared<model::SlideRepository>();
    auto serializer = std::make_shared<serialization::JsonSerializer>();
    auto view = std::make_shared<view::CliView>(std::cout);
    auto inputStream = std::make_shared<io::InputStream>(std::cin);
    
    // controller
    controller::CommandController controller(
        repository,
        serializer,
        view,
        inputStream
    );
    
    // interactive mode
    try {
        controller.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}