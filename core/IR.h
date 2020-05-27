// // Copyright (c) 2020 smarsu. All Rights Reserved.

// #pragma once
// #include <map>

// class SMCCContext {
//  public:
//  private:
// };

// class IR {
//  public:
//   IR() {}

//   virtual ~IR() {}

//  private:
//   void *p;
// };

// class Value {

// };

// class ConstantStr : public Value {
//  public:
//   static std::unique_ptr<ConstantStr> get(SMCCContext context, const std::string &val) {
//     static std::map<std::string, ConstantStr *> constant_strs;
//     if (constant_strs.find(val) == constant_strs.end()) {
//       constant_strs[val] = new ConstantStr(val);
//     }
//     return constant_strs[val];
//   }

//   explicit ConstantStr(const std::string &val)
//     : val_(val) {}

//  private:
//   std::string val_;
// };

// class ConstantFP64 {
//  public:
//   static std::unique_ptr<ConstantFP64> get(SMCCContext context, double val) {
//     static std::map<double, ConstantFP64 *> constant_fp64s;
//     if (constant_fp64s.find(val) == constant_fp64s.end()) {
//       constant_fp64s[val] = new ConstantFP64(val);
//     }
//     return constant_fp64s[val];
//   }

//   explicit ConstantFP64(double val)
//     : val_(val) {}

//  private:
//   double val_;
// };
