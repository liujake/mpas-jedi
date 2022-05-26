/*
 * (C) Copyright 2017-2022 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef MPASJEDI_STATEMPAS_H_
#define MPASJEDI_STATEMPAS_H_

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "oops/base/Variables.h"
#include "oops/util/DateTime.h"
#include "oops/util/ObjectCounter.h"
#include "oops/util/Printable.h"
#include "oops/util/Serializable.h"

#include "mpasjedi/StateMPASFortran.h"
#include "mpasjedi/StateMPASParameters.h"

namespace ufo {
  class GeoVaLs;
  class Locations;
}

namespace oops {
  class Variables;
}

namespace mpas {
  class GeometryMPAS;
  class GetValuesTrajMPAS;
  class IncrementMPAS;

/// MPAS model state
/*!
 * A State contains everything that is needed to propagate the state
 * forward in time.
 */

// -----------------------------------------------------------------------------
class StateMPAS : public util::Printable,
                  public util::Serializable,
                  private util::ObjectCounter<StateMPAS> {
 public:
  static const std::string classname() {return "mpas::StateMPAS";}

  typedef StateMPASParameters Parameters_;
  typedef StateMPASWriteParameters WriteParameters_;

/// Constructor, destructor
  StateMPAS(const GeometryMPAS &, const oops::Variables &,
            const util::DateTime &);  // Is it used?
  StateMPAS(const GeometryMPAS &, const Parameters_ &);
  StateMPAS(const GeometryMPAS &, const StateMPAS &);
  StateMPAS(const StateMPAS &);
  ~StateMPAS();
//  virtual ~StateMPAS();

  StateMPAS & operator=(const StateMPAS &);
  void zero();
  void accumul(const real_type &, const StateMPAS &);

/// Interpolate full fields
  void changeResolution(const StateMPAS & xx);

/// Interactions with Increment
  StateMPAS & operator+=(const IncrementMPAS &);

/// Serialization
  size_t serialSize() const override;
  void serialize(std::vector<real_type> &) const override;
  void deserialize(const std::vector<real_type> &, size_t &) override;

/// I/O and diagnostics
  void read(const Parameters_ &);
  void write(const WriteParameters_ &) const;
  real_type norm() const;

  std::shared_ptr<const GeometryMPAS> geometry() const {return geom_;}

  const util::DateTime & time() const {return time_;}
  util::DateTime & time() {return time_;}
  const util::DateTime & validTime() const {return time_;}
  util::DateTime & validTime() {return time_;}
  const oops::Variables & variables() const {return vars_;}
  void updateTime(const util::Duration & dt) {time_ += dt;}

// Get values as Atlas FieldSet
  void getFieldSet(const oops::Variables &, atlas::FieldSet &) const;

  int & toFortran() {return keyState_;}
  const int & toFortran() const {return keyState_;}

 private:
/// Analytic intialization
  void analytic_init(const Parameters_ &);

  void print(std::ostream &) const override;
  F90state keyState_;
  std::shared_ptr<const GeometryMPAS> geom_;
  oops::Variables vars_;
  util::DateTime time_;

  oops::Variables stateVars();
};

// -----------------------------------------------------------------------------

}  // namespace mpas

#endif  // MPASJEDI_STATEMPAS_H_
