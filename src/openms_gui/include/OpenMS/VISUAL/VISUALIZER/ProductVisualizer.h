// Copyright (c) 2002-present, The OpenMS Team -- EKU Tuebingen, ETH Zurich, and FU Berlin
// SPDX-License-Identifier: BSD-3-Clause
//
// --------------------------------------------------------------------------
// $Maintainer:Timo Sachsenberg $
// $Authors: Marc Sturm $
// --------------------------------------------------------------------------

#pragma once

// OpenMS_GUI config
#include <OpenMS/VISUAL/OpenMS_GUIConfig.h>

//OpenMS
#include <OpenMS/METADATA/Product.h>
#include <OpenMS/VISUAL/VISUALIZER/BaseVisualizer.h>
#include <OpenMS/VISUAL/VISUALIZER/BaseVisualizerGUI.h>

namespace OpenMS
{
  /**
      @brief Class that displays all meta information for Product objects

      This class provides all functionality to view the meta information of an object of type Product.
  */
  class OPENMS_GUI_DLLAPI ProductVisualizer :
    public BaseVisualizerGUI,
    public BaseVisualizer<Product>
  {
    Q_OBJECT

public:

    ///Constructor
    ProductVisualizer(bool editable = false, QWidget * parent = nullptr);

public slots:

    //Docu in base class
    void store() override;

protected slots:

    ///Undo the changes made in the GUI.
    void undo_();

protected:
    ///@name Edit fields and buttons
    //@{
    QLineEdit * product_mz_;
    QLineEdit * product_window_up_;
    QLineEdit * product_window_low_;
    //@}

    //Docu in base class
    void update_() override;
  };
}
