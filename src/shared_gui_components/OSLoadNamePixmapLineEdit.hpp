/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2020-2020, OpenStudio Coalition and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef SHAREDGUICOMPONENTS_OSLOADNAMEPIXMAPLINEEDIT_HPP
#define SHAREDGUICOMPONENTS_OSLOADNAMEPIXMAPLINEEDIT_HPP

#include "FieldMethodTypedefs.hpp"

#include <openstudio/model/Model.hpp>

#include "OSLineEdit.hpp"

class QLabel;

namespace openstudio {

class OSItem;

class OSLoadNamePixmapLineEdit
  : public QWidget
  , public OSLineEdit2Interface
{
  Q_OBJECT

 public:
  OSLoadNamePixmapLineEdit(QWidget* parent = nullptr);

  virtual ~OSLoadNamePixmapLineEdit();

  virtual void enableClickFocus() override;

  virtual bool hasData() override;

  virtual bool locked() const override;

  virtual void setLocked(bool locked) override;

  virtual boost::optional<model::ModelObject> modelObject() const override;

  virtual void bind(const model::ModelObject& modelObject, StringGetter get, boost::optional<StringSetter> set = boost::none,
                    boost::optional<NoFailAction> reset = boost::none, boost::optional<BasicQuery> isDefaulted = boost::none) override;

  virtual void bind(const model::ModelObject& modelObject, OptionalStringGetter get, boost::optional<StringSetter> set = boost::none,
                    boost::optional<NoFailAction> reset = boost::none, boost::optional<BasicQuery> isDefaulted = boost::none) override;

  virtual void bind(const model::ModelObject& modelObject, OptionalStringGetter get,
                    boost::optional<StringSetterOptionalStringReturn> set = boost::none, boost::optional<NoFailAction> reset = boost::none,
                    boost::optional<BasicQuery> isDefaulted = boost::none) override;

  virtual void bind(const model::ModelObject& modelObject, OptionalStringGetterBoolArg get, boost::optional<StringSetterOptionalStringReturn> set,
                    boost::optional<NoFailAction> reset = boost::none, boost::optional<BasicQuery> isDefaulted = boost::none) override;

  virtual void bind(const model::ModelObject& modelObject, StringGetter get, boost::optional<StringSetterVoidReturn> set = boost::none,
                    boost::optional<NoFailAction> reset = boost::none, boost::optional<BasicQuery> isDefaulted = boost::none) override;

  virtual void unbind() override;

  virtual QWidget* qwidget() override;

 signals:

  void itemClicked(OSItem* item);

  void objectRemoved(boost::optional<model::ParentObject> parent);

  void inFocus(bool inFocus, bool hasData);

 public slots:

  virtual void onItemRemoveClicked();

 private:
  void completeBind();
  void createWidgets();
  void setIcon();

  OSLineEdit2* m_lineEdit = nullptr;
  QLabel* m_label = nullptr;
};

}  // namespace openstudio

#endif  // SHAREDGUICOMPONENTS_OSLOADNAMEPIXMAPLINEEDIT_HPP
