#ifndef SLIDEEDITOR_INTERFACES_ICOMMAND_HPP
#define SLIDEEDITOR_INTERFACES_ICOMMAND_HPP

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
};

#endif // SLIDEEDITOR_INTERFACES_ICOMMAND_HPP