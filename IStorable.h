#ifndef ISTORABLE_H
#define ISTORABLE_H

/// TODO: docu
class IStorable
{
   public:
      virtual ~IStorable() {}
      virtual const char* getType() const = 0;
};

class TestObject : public IStorable
{
   public:
      int i;
      const char* getType() const { return "TestObject"; }
};


#endif // ISTORABLE_H
