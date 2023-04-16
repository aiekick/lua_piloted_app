#pragma once

class iVisitor
{
public:
	virtual ~iVisitor() = default;
	virtual void visit() = 0;
};
