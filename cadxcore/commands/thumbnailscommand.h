/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once
#include <api/icommand.h>
#include <api/api.h>

class wxImage;

namespace GADAPI
{
class ThumbnailsCommandParams;

class EXTAPI ThumbnailsNotifier
{
public:
        ThumbnailsNotifier() {}
        virtual ~ThumbnailsNotifier() {}

        virtual void SetImage(long file_pk, wxImage* pImage) = 0;
};

class EXTAPI ThumbnailsCommand : public GNC::GCS::IComando
{
public:
        ThumbnailsCommand(long file_pk, ThumbnailsNotifier* pNotificador);

        wxImage* GetImage();

protected:
        void Execute();
        void Update();

protected:
        wxImage* GetImageFromBD();
        void GuardarImagen();

        ThumbnailsCommandParams* m_pThumbParams;
        bool                            m_Error;

};
}
