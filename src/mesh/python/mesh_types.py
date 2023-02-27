#!/usr/bin/env python
# -------------------------------------------*-python-*------------------------------------------- #
# file  src/mesh/python/mesh_types.py
# date  Monday, Jul 19, 2021, 12:14 pm
# brief This script provides mesh classes that calculate and contain unstructred mesh data.
# note  Copyright (C) 2021-2022 Triad National Security, LLC., All rights reserved.
# ------------------------------------------------------------------------------------------------ #
import numpy as np


# ------------------------------------------------------------------------------------------------ #
# base class (to show required member data)
class base_mesh:
    '''
    Base class with data required by mesh file generators.
    This contains no methods for creating the data members.
    '''

    def __init__(self):
        # -- required data
        self.ndim = 0  # number of dimensions
        self.num_nodes = 0  # total number of nodes
        self.coordinates_per_node = np.array([])  # coordinate array indexed by node
        self.num_cells = 0  # total number of cells
        self.num_faces = 0  # total number of oriented faces
        self.num_faces_per_cell = np.array([], dtype=int)  # number of faces per cell
        self.num_nodes_per_face = np.array([], dtype=int)  # number of nodes per face
        self.faces_per_cell = [np.array([], dtype=int)]  # face indexes per cell
        self.nodes_per_face = [np.array([], dtype=int)]  # node indexes per face
        self.nodes_per_side = [[np.array([], dtype=int)]]  # list of arrays of node per bdy face


# ------------------------------------------------------------------------------------------------ #
# orthogonal 1D mesh type
class orth_1d_mesh(base_mesh):
    '''
    Class for orthogonally structured 1D mesh data.
    This class generates an orthogonally structured mesh in an unstructured
    format suitable for creating unstructured-mesh input files.
    '''

    def __init__(self, bounds_per_dim, num_cells_per_dim):

        # -- short-cuts
        nx = num_cells_per_dim[0]

        # -- number of dimensions
        ndim = len(num_cells_per_dim)
        self.ndim = ndim
        assert (ndim == 1), 'ndim != 1, exiting...'
        assert (len(bounds_per_dim) == ndim), 'len(bounds_per_dim) != ndim, exiting...'

        # create grid arrays along each dimension
        grid_per_dim = [np.linspace(bounds_per_dim[i][0], bounds_per_dim[i][1],
                                    num_cells_per_dim[i] + 1) for i in range(ndim)]

        # -- create node indices
        num_nodes = nx + 1
        self.num_nodes = num_nodes
        self.coordinates_per_node = np.zeros((num_nodes, ndim))
        for i in range(nx + 1):
            node = i
            self.coordinates_per_node[node, 0] = grid_per_dim[0][i]

        # -- set total number of cells and faces
        num_cells = nx
        self.num_cells = num_cells
        self.num_faces = 2 * num_cells

        # -- constants for this mesh
        self.num_faces_per_cell = 2 * np.ones((num_cells), dtype=int)
        self.num_nodes_per_face = np.ones((2 * num_cells), dtype=int)

        # -- set nodes per face and faces per cell
        self.faces_per_cell = np.zeros((num_cells, 2), dtype=int)
        self.nodes_per_face = np.zeros((2 * num_cells, 1), dtype=int)
        for i in range(nx):
            # -- faces per cell
            self.faces_per_cell[i, 0] = 2 * i
            self.faces_per_cell[i, 1] = 2 * i + 1
            # -- nodes per face (per cell)
            self.nodes_per_face[2 * i, 0] = i
            self.nodes_per_face[2 * i + 1, 0] = i + 1

        # -- enumerate boundary nodes per face ("side")
        # -- faces at x extrema
        nodes_per_side_xlow = np.zeros((1, 1), dtype=int)
        nodes_per_side_xhig = np.zeros((1, 1), dtype=int)
        nodes_per_side_xlow[0, 0] = 0
        nodes_per_side_xhig[0, 0] = nx
        # -- compile into one side face array list
        self.nodes_per_side = [nodes_per_side_xlow, nodes_per_side_xhig]


# ------------------------------------------------------------------------------------------------ #
# orthogonal 2D mesh type
class orth_2d_mesh(base_mesh):
    '''
    Class for orthogonally structured 2D mesh data.
    This class generates an orthogonally structured mesh in an unstructured
    format suitable for creating unstructured-mesh input files.
    '''

    def __init__(self, bounds_per_dim, num_cells_per_dim):

        # -- short-cuts
        nx = num_cells_per_dim[0]
        ny = num_cells_per_dim[1]

        # -- number of dimensions
        ndim = len(num_cells_per_dim)
        self.ndim = ndim
        assert (ndim == 2), 'ndim != 2, exiting...'
        assert (len(bounds_per_dim) == ndim), 'len(bounds_per_dim) != ndim, exiting...'

        # -- create grid arrays along each dimension
        grid_per_dim = [np.linspace(bounds_per_dim[i][0], bounds_per_dim[i][1],
                                    num_cells_per_dim[i] + 1) for i in range(ndim)]

        # -- create node indices
        num_nodes = (nx + 1) * (ny + 1)
        self.num_nodes = num_nodes
        self.coordinates_per_node = np.zeros((num_nodes, ndim))
        for j in range(ny + 1):
            j_offset = (nx + 1) * j
            for i in range(nx + 1):
                node = i + j_offset
                self.coordinates_per_node[node, 0] = grid_per_dim[0][i]
                self.coordinates_per_node[node, 1] = grid_per_dim[1][j]

        # -- set total number of cells and faces
        num_cells = nx * ny
        self.num_cells = num_cells
        self.num_faces = 4 * num_cells

        # -- constants for this mesh
        self.num_faces_per_cell = 4 * np.ones((num_cells), dtype=int)
        self.num_nodes_per_face = 2 * np.ones((4 * num_cells), dtype=int)

        # -- set nodes per face and faces per cell
        self.faces_per_cell = np.zeros((num_cells, 4), dtype=int)
        self.nodes_per_face = np.zeros((4 * num_cells, 2), dtype=int)
        for j in range(ny):
            for i in range(nx):
                # -- cell index
                cell = i + nx * j
                # -- faces per cell
                self.faces_per_cell[cell, 0] = 4 * cell
                self.faces_per_cell[cell, 1] = 4 * cell + 1
                self.faces_per_cell[cell, 2] = 4 * cell + 2
                self.faces_per_cell[cell, 3] = 4 * cell + 3
                # -- nodes per face (per cell)
                self.nodes_per_face[4 * cell, 0] = i + j * (nx + 1)
                self.nodes_per_face[4 * cell, 1] = i + 1 + j * (nx + 1)
                self.nodes_per_face[4 * cell + 1, 0] = i + 1 + j * (nx + 1)
                self.nodes_per_face[4 * cell + 1, 1] = i + 1 + (j + 1) * (nx + 1)
                self.nodes_per_face[4 * cell + 2, 0] = i + 1 + (j + 1) * (nx + 1)
                self.nodes_per_face[4 * cell + 2, 1] = i + (j + 1) * (nx + 1)
                self.nodes_per_face[4 * cell + 3, 0] = i + (j + 1) * (nx + 1)
                self.nodes_per_face[4 * cell + 3, 1] = i + j * (nx + 1)

        # -- enumerate boundary nodes per face ("side")
        # -- faces at x extrema
        nodes_per_side_xlow = np.zeros((ny, 2), dtype=int)
        nodes_per_side_xhig = np.zeros((ny, 2), dtype=int)
        for j in range(ny):
            nodes_per_side_xlow[ny - 1 - j, 0] = (j + 1) * (nx + 1)
            nodes_per_side_xlow[ny - 1 - j, 1] = j * (nx + 1)
            nodes_per_side_xhig[j, 0] = j * (nx + 1) + nx
            nodes_per_side_xhig[j, 1] = (j + 1) * (nx + 1) + nx
        # -- faces at y extrema
        nodes_per_side_ylow = np.zeros((nx, 2), dtype=int)
        nodes_per_side_yhig = np.zeros((nx, 2), dtype=int)
        for i in range(nx):
            nodes_per_side_ylow[i, 0] = i
            nodes_per_side_ylow[i, 1] = i + 1
            nodes_per_side_yhig[nx - 1 - i, 0] = i + 1 + ny * (nx + 1)
            nodes_per_side_yhig[nx - 1 - i, 1] = i + ny * (nx + 1)
        # -- compile into one side face array list (expeced as counter-clockwise in 2D)
        self.nodes_per_side = [nodes_per_side_ylow, nodes_per_side_xhig,
                               nodes_per_side_yhig, nodes_per_side_xlow]


# ------------------------------------------------------------------------------------------------ #
# orthogonal 3D mesh type
class orth_3d_mesh(base_mesh):
    '''
    Class for orthogonally structured 3D mesh data.
    This class generates an orthogonally structured mesh in an unstructured
    format suitable for creating unstructured-mesh input files.
    '''

    def __init__(self, bounds_per_dim, num_cells_per_dim):

        # -- short-cuts
        nx = num_cells_per_dim[0]
        ny = num_cells_per_dim[1]
        nz = num_cells_per_dim[2]

        # -- number of dimensions
        ndim = len(num_cells_per_dim)
        self.ndim = ndim
        assert (ndim == 3), 'ndim != 3, exiting...'
        assert (len(bounds_per_dim) == ndim), 'len(bounds_per_dim) != ndim, exiting...'

        # -- create grid arrays along each dimension
        grid_per_dim = [np.linspace(bounds_per_dim[i][0], bounds_per_dim[i][1],
                                    num_cells_per_dim[i] + 1) for i in range(ndim)]

        # -- create node indices
        num_nodes = (nx + 1) * (ny + 1) * (nz + 1)
        self.num_nodes = num_nodes
        self.coordinates_per_node = np.zeros((num_nodes, ndim))
        for k in range(nz + 1):
            k_offset = (nx + 1) * (ny + 1) * k
            for j in range(ny + 1):
                j_offset = (nx + 1) * j
                for i in range(nx + 1):
                    node = i + j_offset + k_offset
                    self.coordinates_per_node[node, 0] = grid_per_dim[0][i]
                    self.coordinates_per_node[node, 1] = grid_per_dim[1][j]
                    self.coordinates_per_node[node, 2] = grid_per_dim[2][k]

        # -- set total number of cells and faces
        num_cells = nx * ny * nz
        self.num_cells = num_cells
        self.num_faces = 6 * num_cells

        # -- constants for this mesh
        self.num_faces_per_cell = 6 * np.ones((num_cells), dtype=int)
        self.num_nodes_per_face = 4 * np.ones((6 * num_cells), dtype=int)

        # -- set nodes per face and faces per cell
        self.faces_per_cell = np.zeros((num_cells, 6), dtype=int)
        self.nodes_per_face = np.zeros((6 * num_cells, 4), dtype=int)
        for k in range(nz):
            kno = k * (ny + 1) * (nx + 1)
            knop1 = (k + 1) * (ny + 1) * (nx + 1)
            for j in range(ny):
                jno = j * (nx + 1)
                jnop1 = (j + 1) * (nx + 1)
                for i in range(nx):
                    # -- cell index
                    cell = i + nx * j + nx * ny * k
                    # -- faces per cell
                    self.faces_per_cell[cell, 0] = 6 * cell
                    self.faces_per_cell[cell, 1] = 6 * cell + 1
                    self.faces_per_cell[cell, 2] = 6 * cell + 2
                    self.faces_per_cell[cell, 3] = 6 * cell + 3
                    self.faces_per_cell[cell, 4] = 6 * cell + 4
                    self.faces_per_cell[cell, 5] = 6 * cell + 5
                    # -- nodes per face (per cell, counterclockwise around face normals) ...
                    # ... bottom face (min z)
                    f = 0
                    self.nodes_per_face[6 * cell + f, 0] = i + jno + kno
                    self.nodes_per_face[6 * cell + f, 1] = i + jnop1 + kno
                    self.nodes_per_face[6 * cell + f, 2] = i + 1 + jnop1 + kno
                    self.nodes_per_face[6 * cell + f, 3] = i + 1 + jno + kno
                    # ... top face (max z)
                    f = 1
                    self.nodes_per_face[6 * cell + f, 0] = i + 1 + jno + knop1
                    self.nodes_per_face[6 * cell + f, 1] = i + 1 + jnop1 + knop1
                    self.nodes_per_face[6 * cell + f, 2] = i + jnop1 + knop1
                    self.nodes_per_face[6 * cell + f, 3] = i + jno + knop1
                    # ... down face (min y)
                    f = 2
                    self.nodes_per_face[6 * cell + f, 0] = i + jno + kno
                    self.nodes_per_face[6 * cell + f, 1] = i + 1 + jno + kno
                    self.nodes_per_face[6 * cell + f, 2] = i + 1 + jno + knop1
                    self.nodes_per_face[6 * cell + f, 3] = i + jno + knop1
                    # ... up face (max y)
                    f = 3
                    self.nodes_per_face[6 * cell + f, 0] = i + jnop1 + knop1
                    self.nodes_per_face[6 * cell + f, 1] = i + 1 + jnop1 + knop1
                    self.nodes_per_face[6 * cell + f, 2] = i + 1 + jnop1 + kno
                    self.nodes_per_face[6 * cell + f, 3] = i + jnop1 + kno
                    # ... strange face (min x)
                    f = 4
                    self.nodes_per_face[6 * cell + f, 0] = i + jno + kno
                    self.nodes_per_face[6 * cell + f, 1] = i + jno + knop1
                    self.nodes_per_face[6 * cell + f, 2] = i + jnop1 + knop1
                    self.nodes_per_face[6 * cell + f, 3] = i + jnop1 + kno
                    # ... charm face (max x)
                    f = 5
                    self.nodes_per_face[6 * cell + f, 0] = i + 1 + jnop1 + kno
                    self.nodes_per_face[6 * cell + f, 1] = i + 1 + jnop1 + knop1
                    self.nodes_per_face[6 * cell + f, 2] = i + 1 + jno + knop1
                    self.nodes_per_face[6 * cell + f, 3] = i + 1 + jno + kno

        # -- enumerate boundary nodes per face ("side")

        # -- faces at x extrema
        nodes_per_side_xlow = np.zeros((nz * ny, 4), dtype=int)
        nodes_per_side_xhig = np.zeros((nz * ny, 4), dtype=int)
        for k in range(nz):
            kno = k * (ny + 1) * (nx + 1)
            knop1 = (k + 1) * (ny + 1) * (nx + 1)
            for j in range(ny):
                f = j + ny * k
                # -- low boundary
                nodes_per_side_xlow[f, 0] = (nx + 1) * j + kno
                nodes_per_side_xlow[f, 1] = (nx + 1) * j + knop1
                nodes_per_side_xlow[f, 2] = (nx + 1) * (j + 1) + knop1
                nodes_per_side_xlow[f, 3] = (nx + 1) * (j + 1) + kno
                # -- high boundary
                nodes_per_side_xhig[f, 0] = nx + (nx + 1) * (j + 1) + kno
                nodes_per_side_xhig[f, 1] = nx + (nx + 1) * (j + 1) + knop1
                nodes_per_side_xhig[f, 2] = nx + (nx + 1) * j + knop1
                nodes_per_side_xhig[f, 3] = nx + (nx + 1) * j + kno

        # -- faces at y extrema
        nodes_per_side_ylow = np.zeros((nx * nz, 4), dtype=int)
        nodes_per_side_yhig = np.zeros((nx * nz, 4), dtype=int)
        for k in range(nz):
            kno = k * (ny + 1) * (nx + 1)
            knop1 = (k + 1) * (ny + 1) * (nx + 1)
            for i in range(nx):
                f = i + nx * k
                # -- low boundary
                nodes_per_side_ylow[f, 0] = i + kno
                nodes_per_side_ylow[f, 1] = i + 1 + kno
                nodes_per_side_ylow[f, 2] = i + 1 + knop1
                nodes_per_side_ylow[f, 3] = i + knop1
                # -- high boundary
                nodes_per_side_yhig[f, 0] = i + (nx + 1) * ny + knop1
                nodes_per_side_yhig[f, 1] = i + 1 + (nx + 1) * ny + knop1
                nodes_per_side_yhig[f, 2] = i + 1 + (nx + 1) * ny + kno
                nodes_per_side_yhig[f, 3] = i + (nx + 1) * ny + kno

        # -- faces at z extrema
        nodes_per_side_zlow = np.zeros((ny * nx, 4), dtype=int)
        nodes_per_side_zhig = np.zeros((ny * nx, 4), dtype=int)
        for j in range(ny):
            jno = j * (nx + 1)
            jnop1 = (j + 1) * (nx + 1)
            for i in range(nx):
                f = i + nx * j
                # -- low boundary
                nodes_per_side_zlow[f, 0] = i + jno
                nodes_per_side_zlow[f, 1] = i + jnop1
                nodes_per_side_zlow[f, 2] = i + 1 + jnop1
                nodes_per_side_zlow[f, 3] = i + 1 + jno
                # -- high boundary
                nodes_per_side_zhig[f, 0] = i + 1 + jno + nz * (ny + 1) * (nx + 1)
                nodes_per_side_zhig[f, 1] = i + 1 + jnop1 + nz * (ny + 1) * (nx + 1)
                nodes_per_side_zhig[f, 2] = i + jnop1 + nz * (ny + 1) * (nx + 1)
                nodes_per_side_zhig[f, 3] = i + jno + nz * (ny + 1) * (nx + 1)

        # -- compile into one side face array list
        self.nodes_per_side = [nodes_per_side_xlow, nodes_per_side_xhig,
                               nodes_per_side_ylow, nodes_per_side_yhig,
                               nodes_per_side_zlow, nodes_per_side_zhig]


# ------------------------------------------------------------------------------------------------ #
# face-centered cubic (fcc) 3D mesh type
class fcc_3d_mesh(base_mesh):
    '''
    Class for structured 3D mesh data with face-centered cubic vertex configurations.
    '''

    def __init__(self, bounds_per_dim, num_cells_per_dim):

        # -- short-cuts
        nx = num_cells_per_dim[0]
        ny = num_cells_per_dim[1]
        nz = num_cells_per_dim[2]

        # -- number of dimensions
        ndim = len(num_cells_per_dim)
        self.ndim = ndim
        assert (ndim == 3), 'ndim != 3, exiting...'
        assert (len(bounds_per_dim) == ndim), 'len(bounds_per_dim) != ndim, exiting...'

        # -- create grid arrays along each dimension
        grid_per_dim = [np.linspace(bounds_per_dim[i][0], bounds_per_dim[i][1],
                                    2 * num_cells_per_dim[i] + 1) for i in range(ndim)]

        # -- create node indices ...
        # ... number of simple cubic nodes
        num_sc_nodes = (nx + 1) * (ny + 1) * (nz + 1)
        # ... number of face-centered nodes
        num_fc_nodes = (nz + 1) * nx * ny + (ny + 1) * nz * nx + (nx + 1) * ny * nz
        # ... total number of nodes
        num_nodes = num_sc_nodes + num_fc_nodes
        self.num_nodes = num_nodes
        self.coordinates_per_node = np.zeros((num_nodes, ndim))
        # ... populate node coordinates for simple nodes
        for k in range(nz + 1):
            k_offset = (nx + 1) * (ny + 1) * k
            for j in range(ny + 1):
                j_offset = (nx + 1) * j
                for i in range(nx + 1):
                    node = i + j_offset + k_offset
                    self.coordinates_per_node[node, 0] = grid_per_dim[0][2 * i]
                    self.coordinates_per_node[node, 1] = grid_per_dim[1][2 * j]
                    self.coordinates_per_node[node, 2] = grid_per_dim[2][2 * k]
        # ... populate nodes on z-normal (xy-plane) faces
        for k in range(nz + 1):
            k_offset = nx * ny * k
            for j in range(ny):
                j_offset = nx * j
                for i in range(nx):
                    node = i + j_offset + k_offset + num_sc_nodes
                    self.coordinates_per_node[node, 0] = grid_per_dim[0][2 * i + 1]
                    self.coordinates_per_node[node, 1] = grid_per_dim[1][2 * j + 1]
                    self.coordinates_per_node[node, 2] = grid_per_dim[2][2 * k]
        # ... populate nodes on y-normal (zx-plane) faces
        for k in range(nz):
            k_offset = nx * (ny + 1) * k
            for j in range(ny + 1):
                j_offset = nx * j
                for i in range(nx):
                    sc_offset = num_sc_nodes + (nz + 1) * nx * ny
                    node = i + j_offset + k_offset + sc_offset
                    self.coordinates_per_node[node, 0] = grid_per_dim[0][2 * i + 1]
                    self.coordinates_per_node[node, 1] = grid_per_dim[1][2 * j]
                    self.coordinates_per_node[node, 2] = grid_per_dim[2][2 * k + 1]
        # ... populate nodes on x-normal (yz-plane) faces
        for k in range(nz):
            k_offset = (nx + 1) * ny * k
            for j in range(ny):
                j_offset = (nx + 1) * j
                for i in range(nx + 1):
                    sc_offset = num_sc_nodes + (nz + 1) * nx * ny + (ny + 1) * nz * nx
                    node = i + j_offset + k_offset + sc_offset
                    self.coordinates_per_node[node, 0] = grid_per_dim[0][2 * i]
                    self.coordinates_per_node[node, 1] = grid_per_dim[1][2 * j + 1]
                    self.coordinates_per_node[node, 2] = grid_per_dim[2][2 * k + 1]

        # -- set total number of cells and faces
        num_cells = nx * ny * nz
        self.num_cells = num_cells
        self.num_faces = 24 * num_cells

        # -- constants for this mesh
        self.num_faces_per_cell = 24 * np.ones((num_cells), dtype=int)
        self.num_nodes_per_face = 3 * np.ones((24 * num_cells), dtype=int)

        # -- set nodes per face and faces per cell
        self.faces_per_cell = np.zeros((num_cells, 24), dtype=int)
        self.nodes_per_face = np.zeros((24 * num_cells, 3), dtype=int)
        for k in range(nz):
            kno = k * (ny + 1) * (nx + 1)
            knop1 = (k + 1) * (ny + 1) * (nx + 1)
            for j in range(ny):
                jno = j * (nx + 1)
                jnop1 = (j + 1) * (nx + 1)
                for i in range(nx):
                    # -- cell index
                    cell = i + nx * j + nx * ny * k
                    # -- faces per cell
                    for ll in range(24):
                        self.faces_per_cell[cell, ll] = 24 * cell + ll
                    # -- nodes per face (per cell, counterclockwise around face normals) ...
                    # ... bottom faces (min z)
                    fc_node = i + nx * j + nx * ny * k + num_sc_nodes
                    f = 0
                    self.nodes_per_face[24 * cell + f, 0] = i + jno + kno
                    self.nodes_per_face[24 * cell + f, 1] = i + jnop1 + kno
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 1
                    self.nodes_per_face[24 * cell + f, 0] = i + jnop1 + kno
                    self.nodes_per_face[24 * cell + f, 1] = i + 1 + jnop1 + kno
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 2
                    self.nodes_per_face[24 * cell + f, 0] = i + 1 + jnop1 + kno
                    self.nodes_per_face[24 * cell + f, 1] = i + 1 + jno + kno
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 3
                    self.nodes_per_face[24 * cell + f, 0] = i + 1 + jno + kno
                    self.nodes_per_face[24 * cell + f, 1] = i + jno + kno
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    # ... top face (max z)
                    fc_node = i + nx * j + nx * ny * (k + 1) + num_sc_nodes
                    f = 4
                    self.nodes_per_face[24 * cell + f, 0] = i + 1 + jno + knop1
                    self.nodes_per_face[24 * cell + f, 1] = i + 1 + jnop1 + knop1
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 5
                    self.nodes_per_face[24 * cell + f, 0] = i + 1 + jnop1 + knop1
                    self.nodes_per_face[24 * cell + f, 1] = i + jnop1 + knop1
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 6
                    self.nodes_per_face[24 * cell + f, 0] = i + jnop1 + knop1
                    self.nodes_per_face[24 * cell + f, 1] = i + jno + knop1
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 7
                    self.nodes_per_face[24 * cell + f, 0] = i + jno + knop1
                    self.nodes_per_face[24 * cell + f, 1] = i + 1 + jno + knop1
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    # ... down face (min y)
                    sc_offset = num_sc_nodes + (nz + 1) * nx * ny
                    fc_node = i + nx * j + nx * (ny + 1) * k + sc_offset
                    f = 8
                    self.nodes_per_face[24 * cell + f, 0] = i + jno + kno
                    self.nodes_per_face[24 * cell + f, 1] = i + 1 + jno + kno
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 9
                    self.nodes_per_face[24 * cell + f, 0] = i + 1 + jno + kno
                    self.nodes_per_face[24 * cell + f, 1] = i + 1 + jno + knop1
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 10
                    self.nodes_per_face[24 * cell + f, 0] = i + 1 + jno + knop1
                    self.nodes_per_face[24 * cell + f, 1] = i + jno + knop1
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 11
                    self.nodes_per_face[24 * cell + f, 0] = i + jno + knop1
                    self.nodes_per_face[24 * cell + f, 1] = i + jno + kno
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    # ... up face (max y)
                    sc_offset = num_sc_nodes + (nz + 1) * nx * ny
                    fc_node = i + nx * (j + 1) + nx * (ny + 1) * k + sc_offset
                    f = 12
                    self.nodes_per_face[24 * cell + f, 0] = i + jnop1 + knop1
                    self.nodes_per_face[24 * cell + f, 1] = i + 1 + jnop1 + knop1
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 13
                    self.nodes_per_face[24 * cell + f, 0] = i + 1 + jnop1 + knop1
                    self.nodes_per_face[24 * cell + f, 1] = i + 1 + jnop1 + kno
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 14
                    self.nodes_per_face[24 * cell + f, 0] = i + 1 + jnop1 + kno
                    self.nodes_per_face[24 * cell + f, 1] = i + jnop1 + kno
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 15
                    self.nodes_per_face[24 * cell + f, 0] = i + jnop1 + kno
                    self.nodes_per_face[24 * cell + f, 1] = i + jnop1 + knop1
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    # ... strange face (min x)
                    sc_offset = num_sc_nodes + (nz + 1) * nx * ny + (ny + 1) * nz * nx
                    fc_node = i + (nx + 1) * j + (nx + 1) * ny * k + sc_offset
                    f = 16
                    self.nodes_per_face[24 * cell + f, 0] = i + jno + kno
                    self.nodes_per_face[24 * cell + f, 1] = i + jno + knop1
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 17
                    self.nodes_per_face[24 * cell + f, 0] = i + jno + knop1
                    self.nodes_per_face[24 * cell + f, 1] = i + jnop1 + knop1
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 18
                    self.nodes_per_face[24 * cell + f, 0] = i + jnop1 + knop1
                    self.nodes_per_face[24 * cell + f, 1] = i + jnop1 + kno
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 19
                    self.nodes_per_face[24 * cell + f, 0] = i + jnop1 + kno
                    self.nodes_per_face[24 * cell + f, 1] = i + jno + kno
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    # ... charm face (max x)
                    sc_offset = num_sc_nodes + (nz + 1) * nx * ny + (ny + 1) * nz * nx
                    fc_node = i + 1 + (nx + 1) * j + (nx + 1) * ny * k + sc_offset
                    f = 20
                    self.nodes_per_face[24 * cell + f, 0] = i + 1 + jnop1 + kno
                    self.nodes_per_face[24 * cell + f, 1] = i + 1 + jnop1 + knop1
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 21
                    self.nodes_per_face[24 * cell + f, 0] = i + 1 + jnop1 + knop1
                    self.nodes_per_face[24 * cell + f, 1] = i + 1 + jno + knop1
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 22
                    self.nodes_per_face[24 * cell + f, 0] = i + 1 + jno + knop1
                    self.nodes_per_face[24 * cell + f, 1] = i + 1 + jno + kno
                    self.nodes_per_face[24 * cell + f, 2] = fc_node
                    f = 23
                    self.nodes_per_face[24 * cell + f, 0] = i + 1 + jno + kno
                    self.nodes_per_face[24 * cell + f, 1] = i + 1 + jnop1 + kno
                    self.nodes_per_face[24 * cell + f, 2] = fc_node

        # -- enumerate boundary nodes per face ("side")

        # -- faces at x extrema
        nodes_per_side_xlow = np.zeros((4 * nz * ny, 3), dtype=int)
        nodes_per_side_xhig = np.zeros((4 * nz * ny, 3), dtype=int)
        for k in range(nz):
            kno = k * (ny + 1) * (nx + 1)
            knop1 = (k + 1) * (ny + 1) * (nx + 1)
            for j in range(ny):
                f = j + ny * k
                sc_offset = num_sc_nodes + (nz + 1) * nx * ny + (ny + 1) * nz * nx
                fc_node_xlow = (nx + 1) * j + (nx + 1) * ny * k + sc_offset
                fc_node_xhig = nx + (nx + 1) * j + (nx + 1) * ny * k + sc_offset
                # -- low boundary
                nodes_per_side_xlow[4 * f, 0] = (nx + 1) * j + kno
                nodes_per_side_xlow[4 * f, 1] = (nx + 1) * j + knop1
                nodes_per_side_xlow[4 * f, 2] = fc_node_xlow
                nodes_per_side_xlow[4 * f + 1, 0] = (nx + 1) * j + knop1
                nodes_per_side_xlow[4 * f + 1, 1] = (nx + 1) * (j + 1) + knop1
                nodes_per_side_xlow[4 * f + 1, 2] = fc_node_xlow
                nodes_per_side_xlow[4 * f + 2, 0] = (nx + 1) * (j + 1) + knop1
                nodes_per_side_xlow[4 * f + 2, 1] = (nx + 1) * (j + 1) + kno
                nodes_per_side_xlow[4 * f + 2, 2] = fc_node_xlow
                nodes_per_side_xlow[4 * f + 3, 0] = (nx + 1) * (j + 1) + kno
                nodes_per_side_xlow[4 * f + 3, 1] = (nx + 1) * j + kno
                nodes_per_side_xlow[4 * f + 3, 2] = fc_node_xlow
                # -- high boundary
                nodes_per_side_xhig[4 * f, 0] = nx + (nx + 1) * (j + 1) + kno
                nodes_per_side_xhig[4 * f, 1] = nx + (nx + 1) * (j + 1) + knop1
                nodes_per_side_xhig[4 * f, 2] = fc_node_xhig
                nodes_per_side_xhig[4 * f + 1, 0] = nx + (nx + 1) * (j + 1) + knop1
                nodes_per_side_xhig[4 * f + 1, 1] = nx + (nx + 1) * j + knop1
                nodes_per_side_xhig[4 * f + 1, 2] = fc_node_xhig
                nodes_per_side_xhig[4 * f + 2, 0] = nx + (nx + 1) * j + knop1
                nodes_per_side_xhig[4 * f + 2, 1] = nx + (nx + 1) * j + kno
                nodes_per_side_xhig[4 * f + 2, 2] = fc_node_xhig
                nodes_per_side_xhig[4 * f + 3, 0] = nx + (nx + 1) * j + kno
                nodes_per_side_xhig[4 * f + 3, 1] = nx + (nx + 1) * (j + 1) + kno
                nodes_per_side_xhig[4 * f + 3, 2] = fc_node_xhig

        # -- faces at y extrema
        nodes_per_side_ylow = np.zeros((4 * nx * nz, 3), dtype=int)
        nodes_per_side_yhig = np.zeros((4 * nx * nz, 3), dtype=int)
        for k in range(nz):
            kno = k * (ny + 1) * (nx + 1)
            knop1 = (k + 1) * (ny + 1) * (nx + 1)
            for i in range(nx):
                f = i + nx * k
                sc_offset = num_sc_nodes + (nz + 1) * nx * ny
                fc_node_ylow = i + nx * (ny + 1) * k + sc_offset
                fc_node_yhig = i + nx * ny + nx * (ny + 1) * k + sc_offset
                # -- low boundary
                nodes_per_side_ylow[4 * f, 0] = i + kno
                nodes_per_side_ylow[4 * f, 1] = i + 1 + kno
                nodes_per_side_ylow[4 * f, 2] = fc_node_ylow
                nodes_per_side_ylow[4 * f + 1, 0] = i + 1 + kno
                nodes_per_side_ylow[4 * f + 1, 1] = i + 1 + knop1
                nodes_per_side_ylow[4 * f + 1, 2] = fc_node_ylow
                nodes_per_side_ylow[4 * f + 2, 0] = i + 1 + knop1
                nodes_per_side_ylow[4 * f + 2, 1] = i + knop1
                nodes_per_side_ylow[4 * f + 2, 2] = fc_node_ylow
                nodes_per_side_ylow[4 * f + 3, 0] = i + knop1
                nodes_per_side_ylow[4 * f + 3, 1] = i + kno
                nodes_per_side_ylow[4 * f + 3, 2] = fc_node_ylow
                # -- high boundary
                nodes_per_side_yhig[4 * f, 0] = i + (nx + 1) * ny + knop1
                nodes_per_side_yhig[4 * f, 1] = i + 1 + (nx + 1) * ny + knop1
                nodes_per_side_yhig[4 * f, 2] = fc_node_yhig
                nodes_per_side_yhig[4 * f + 1, 0] = i + 1 + (nx + 1) * ny + knop1
                nodes_per_side_yhig[4 * f + 1, 1] = i + 1 + (nx + 1) * ny + kno
                nodes_per_side_yhig[4 * f + 1, 2] = fc_node_yhig
                nodes_per_side_yhig[4 * f + 2, 0] = i + 1 + (nx + 1) * ny + kno
                nodes_per_side_yhig[4 * f + 2, 1] = i + (nx + 1) * ny + kno
                nodes_per_side_yhig[4 * f + 2, 2] = fc_node_yhig
                nodes_per_side_yhig[4 * f + 3, 0] = i + (nx + 1) * ny + kno
                nodes_per_side_yhig[4 * f + 3, 1] = i + (nx + 1) * ny + knop1
                nodes_per_side_yhig[4 * f + 3, 2] = fc_node_yhig

        # -- faces at z extrema
        nodes_per_side_zlow = np.zeros((4 * ny * nx, 3), dtype=int)
        nodes_per_side_zhig = np.zeros((4 * ny * nx, 3), dtype=int)
        for j in range(ny):
            jno = j * (nx + 1)
            jnop1 = (j + 1) * (nx + 1)
            for i in range(nx):
                f = i + nx * j
                fc_node_zlow = i + nx * j + num_sc_nodes
                fc_node_zhig = i + nx * j + nx * ny * nz + num_sc_nodes
                # -- low boundary
                nodes_per_side_zlow[4 * f, 0] = i + jno
                nodes_per_side_zlow[4 * f, 1] = i + jnop1
                nodes_per_side_zlow[4 * f, 2] = fc_node_zlow
                nodes_per_side_zlow[4 * f + 1, 0] = i + jnop1
                nodes_per_side_zlow[4 * f + 1, 1] = i + 1 + jnop1
                nodes_per_side_zlow[4 * f + 1, 2] = fc_node_zlow
                nodes_per_side_zlow[4 * f + 2, 0] = i + 1 + jnop1
                nodes_per_side_zlow[4 * f + 2, 1] = i + 1 + jno
                nodes_per_side_zlow[4 * f + 2, 2] = fc_node_zlow
                nodes_per_side_zlow[4 * f + 3, 0] = i + 1 + jno
                nodes_per_side_zlow[4 * f + 3, 1] = i + jno
                nodes_per_side_zlow[4 * f + 3, 2] = fc_node_zlow
                # -- high boundary
                nodes_per_side_zhig[4 * f, 0] = i + 1 + jno + nz * (ny + 1) * (nx + 1)
                nodes_per_side_zhig[4 * f, 1] = i + 1 + jnop1 + nz * (ny + 1) * (nx + 1)
                nodes_per_side_zhig[4 * f, 2] = fc_node_zhig
                nodes_per_side_zhig[4 * f + 1, 0] = i + 1 + jnop1 + nz * (ny + 1) * (nx + 1)
                nodes_per_side_zhig[4 * f + 1, 1] = i + jnop1 + nz * (ny + 1) * (nx + 1)
                nodes_per_side_zhig[4 * f + 1, 2] = fc_node_zhig
                nodes_per_side_zhig[4 * f + 2, 0] = i + jnop1 + nz * (ny + 1) * (nx + 1)
                nodes_per_side_zhig[4 * f + 2, 1] = i + jno + nz * (ny + 1) * (nx + 1)
                nodes_per_side_zhig[4 * f + 2, 2] = fc_node_zhig
                nodes_per_side_zhig[4 * f + 3, 0] = i + jno + nz * (ny + 1) * (nx + 1)
                nodes_per_side_zhig[4 * f + 3, 1] = i + 1 + jno + nz * (ny + 1) * (nx + 1)
                nodes_per_side_zhig[4 * f + 3, 2] = fc_node_zhig

        # -- compile into one side face array list
        self.nodes_per_side = [nodes_per_side_xlow, nodes_per_side_xhig,
                               nodes_per_side_ylow, nodes_per_side_yhig,
                               nodes_per_side_zlow, nodes_per_side_zhig]

# ------------------------------------------------------------------------------------------------ #
# Voronoi 2D mesh type


class vor_2d_mesh(base_mesh):
    '''
    Class for randomly generated 2D Voronoi mesh data.
    This class generates a set of random nodes inside a bounding box, and then
    constructs a mesh from the Voronoi diagram of these points. It casts this
    mesh in an unstructured format suitable for creating unstructured-mesh input
    files.
    '''

    def __init__(self, bounds_per_dim, num_cells, seed=0):
        def soft_equiv(a, b, eps=1.e-12):
            if 2. * np.fabs(a - b) / (a + b + eps) < eps:
                return True
            else:
                return False
        from scipy.spatial import Voronoi
        np.random.seed(seed)

        # -- convenience variables
        xmin = bounds_per_dim[0][0]
        xmax = bounds_per_dim[0][1]
        ymin = bounds_per_dim[1][0]
        ymax = bounds_per_dim[1][1]

        # -- number of dimensions
        ndim = len(bounds_per_dim)
        self.ndim = ndim
        self.num_cells = num_cells
        assert (ndim == 2), 'ndim != 2, exiting...'

        # -- randomly distribute grid-generating points inside bounding box
        points = np.zeros([num_cells, 2])
        for n in range(num_cells):
            points[n, 0] = xmin + (xmax - xmin) * np.random.uniform()
            points[n, 1] = ymin + (ymax - ymin) * np.random.uniform()

        # -- create base Voronoi diagram
        vor = Voronoi(points)

        # -- sanitize Voronoi diagram (remove empty regions, remove vertices and
        #    edges outside of domain
        points = vor.points
        vertices = []
        ridge_points = []
        ridge_vertices = []

        new_to_old_vertex_indices = []
        old_to_new_vertex_indices = []
        for n, vertex in enumerate(vor.vertices):
            x = vertex[0]
            y = vertex[1]
            if not (x < xmin or x > xmax or y < ymin or y > ymax):
                vertices.append(vertex.tolist())
                new_to_old_vertex_indices.append(n)
            if n in new_to_old_vertex_indices:
                old_to_new_vertex_indices.append(new_to_old_vertex_indices.index(n))
            else:
                old_to_new_vertex_indices.append(-1)

        # -- update vertex indices for ridge_vertices
        ridge_vertices = vor.ridge_vertices
        for n in range(len(ridge_vertices)):
            for m in range(2):
                if ridge_vertices[n][m] != -1:
                    ridge_vertices[n][m] = old_to_new_vertex_indices[ridge_vertices[n][m]]

        # -- remove ridges from ridge_vertices and ridge_points if [-1,-1] i.e.
        #    entirely outside of bounding box
        ridge_points = vor.ridge_points.tolist()
        tmp_ridge_vertices = []
        tmp_ridge_points = []
        for n in range(len(ridge_vertices)):
            if not (ridge_vertices[n][0] == -1 and ridge_vertices[n][1] == -1):
                tmp_ridge_vertices.append(ridge_vertices[n])
                tmp_ridge_points.append(ridge_points[n])
        ridge_vertices = tmp_ridge_vertices
        ridge_points = tmp_ridge_points

        # -- helper function for logic identifying which direction to find
        #    boundary intersections in
        def ray_segment_intersection(origin, direction, pt1, pt2):
            def mag(v):
                return np.sqrt(np.dot(np.array(v), np.array(v)))

            def norm(v):
                return np.array(v) / mag(v)
            origin = np.array(origin)
            direction = np.array(norm(direction))
            pt1 = np.array(pt1)
            pt2 = np.array(pt2)

            v1 = origin - pt1
            v2 = pt2 - pt1
            v3 = np.array([-direction[1], direction[0]])
            t1 = np.cross(v2, v1) / np.dot(v2, v3)
            t2 = np.dot(v1, v3) / np.dot(v2, v3)
            if t1 >= 0. and t2 >= 0. and t2 <= 1.:
                return origin + t1 * direction
            else:
                return None

        # -- add boundary vertices for ridges with a -1 (i.e. missing) vertex,
        #    with some rather odd logic for ensuring that we cast rays from
        #    existing vertices towards the correct boundary
        new_vertices_start_index = len(vertices)
        for n, ridge in enumerate(ridge_vertices):
            bad_vertex = None
            if ridge[0] == -1:
                good_vertex = 1
                bad_vertex = 0
            if ridge[1] == -1:
                good_vertex = 0
                bad_vertex = 1

            if bad_vertex is not None:
                # -- determine which cells own this ridge
                point1 = points[ridge_points[n][0]]
                point2 = points[ridge_points[n][1]]
                midpoint = [(point1[0] + point2[0]) / 2, (point1[1] + point2[1]) / 2]
                distances = np.zeros(4)
                bpt1s = [[xmin, xmin], [xmax, xmax], [xmin, xmax], [xmin, xmax]]
                bpt2s = [[ymin, ymax], [ymin, ymax], [ymin, ymin], [ymax, ymax]]

                origin = [vertices[ridge[good_vertex]][0], vertices[ridge[good_vertex]][1]]
                direction = [midpoint[0] - vertices[ridge[good_vertex]][0],
                             midpoint[1] - vertices[ridge[good_vertex]][1]]
                found = False
                for bc in range(4):
                    pt1 = [bpt1s[bc][0], bpt2s[bc][0]]
                    pt2 = [bpt1s[bc][1], bpt2s[bc][1]]
                    # -- check reverse directions only if forward directions don't work
                    for d in range(1):
                        if d == 1:
                            direction[0] = -direction[0]
                            direction[1] = -direction[1]
                        newpt = ray_segment_intersection(origin, direction, pt1, pt2)
                        if newpt is not None:
                            # -- check for intersection with other faces
                            bad = False
                            for tmpridge in ridge_vertices:
                                if (tmpridge[0] == len(vertices) - 1 or
                                        tmpridge[1] == len(vertices) - 1 or
                                        tmpridge[0] == ridge[good_vertex] or
                                        tmpridge[1] == ridge[good_vertex] or
                                        tmpridge[0] == -1 or tmpridge[1] == -1):
                                    continue
                                tstpt = ray_segment_intersection(origin, direction,
                                                                 [vertices[tmpridge[0]][0],
                                                                  vertices[tmpridge[0]][1]],
                                                                 [vertices[tmpridge[1]][0],
                                                                     vertices[tmpridge[1]][1]])
                                if tstpt is not None:
                                    bad = True
                                    break
                            if not bad:
                                vertices.append([newpt[0], newpt[1]])
                                ridge_vertices[n][bad_vertex] = len(vertices) - 1
                                found = True
                if found is False:
                    direction[0] = -direction[0]
                    direction[1] = -direction[1]
                    for bc in range(4):
                        pt1 = [bpt1s[bc][0], bpt2s[bc][0]]
                        pt2 = [bpt1s[bc][1], bpt2s[bc][1]]
                        for d in range(1, 2):
                            newpt = ray_segment_intersection(origin, direction, pt1, pt2)
                            if newpt is not None:
                                # -- check for intersection with other faces
                                bad = False
                                for tmpridge in ridge_vertices:
                                    if (tmpridge[0] == len(vertices) - 1 or
                                            tmpridge[1] == len(vertices) - 1 or
                                            tmpridge[0] == ridge[good_vertex] or
                                            tmpridge[1] == ridge[good_vertex] or
                                            tmpridge[0] == -1 or tmpridge[1] == -1):
                                        continue
                                    tstpt = ray_segment_intersection(origin, direction,
                                                                     [vertices[tmpridge[0]][0],
                                                                      vertices[tmpridge[0]][1]],
                                                                     [vertices[tmpridge[1]][0],
                                                                         vertices[tmpridge[1]][1]])
                                    if tstpt is not None:
                                        bad = True
                                        break
                                if not bad:
                                    vertices.append([newpt[0], newpt[1]])
                                    ridge_vertices[n][bad_vertex] = len(vertices) - 1
                                    found = True
                    assert (found is not False), 'No suitable boundary point found!'
                continue

        # -- add boundary edges, ignoring corner vertices for now
        points_to_work_on = []
        points_to_work_on_vertices = []
        for n, ridge in enumerate(ridge_vertices):
            v1 = ridge[0]
            v2 = ridge[1]
            if v1 >= new_vertices_start_index or v2 >= new_vertices_start_index:
                if v1 >= new_vertices_start_index:
                    new = v1
                else:
                    new = v2
                for point in ridge_points[n]:
                    if point not in points_to_work_on:
                        points_to_work_on.append(point)
                        points_to_work_on_vertices.append([new, -1])
                    else:
                        idx = points_to_work_on.index(point)
                        points_to_work_on_vertices[idx][1] = new

        # -- add corners
        for verts in points_to_work_on_vertices:
            v1 = vertices[verts[0]]
            v2 = vertices[verts[1]]
            # -- bottom left
            if (soft_equiv(v1[0], xmin) or soft_equiv(v2[0], xmin)) and \
               (soft_equiv(v1[1], ymin) or soft_equiv(v2[1], ymin)):
                vertices.append([xmin, ymin])
                ridge_vertices.append([verts[0], len(vertices) - 1])
                ridge_vertices.append([verts[1], len(vertices) - 1])
            # -- top left
            elif (soft_equiv(v1[0], xmin) or soft_equiv(v2[0], xmin)) and \
                 (soft_equiv(v1[1], ymax) or soft_equiv(v2[1], ymax)):
                vertices.append([xmin, ymax])
                ridge_vertices.append([verts[0], len(vertices) - 1])
                ridge_vertices.append([verts[1], len(vertices) - 1])
            # -- bottom right
            elif (soft_equiv(v1[0], xmax) or soft_equiv(v2[0], xmax)) and \
                 (soft_equiv(v1[1], ymin) or soft_equiv(v2[1], ymin)):
                vertices.append([xmax, ymin])
                ridge_vertices.append([verts[0], len(vertices) - 1])
                ridge_vertices.append([verts[1], len(vertices) - 1])
            # -- top right
            elif (soft_equiv(v1[0], xmax) or soft_equiv(v2[0], xmax)) and \
                 (soft_equiv(v1[1], ymax) or soft_equiv(v2[1], ymax)):
                vertices.append([xmax, ymax])
                ridge_vertices.append([verts[0], len(vertices) - 1])
                ridge_vertices.append([verts[1], len(vertices) - 1])
            # -- not a corner
            else:
                ridge_vertices.append(verts)

        # -- assign ridge vertices to regions
        cells = []
        cell_nodes = []
        for n, point in enumerate(points):
            cell_nodes.append(n)
            cells.append([])
        for ridge_idx, v_indices in enumerate(ridge_vertices):
            midpoint = [(vertices[v_indices[0]][0] + vertices[v_indices[1]][0]) / 2,
                        (vertices[v_indices[0]][1] + vertices[v_indices[1]][1]) / 2]
            distances = np.zeros(len(cell_nodes))
            for n, node in enumerate(cell_nodes):
                distances[n] = np.sqrt((points[n][0] - midpoint[0])**2 +
                                       (points[n][1] - midpoint[1])**2)
            indices = np.argsort(distances)
            distances = np.sort(distances)
            if soft_equiv(distances[0], distances[1]):
                # -- not a boundary
                cells[indices[0]].append(ridge_idx)
                cells[indices[1]].append(ridge_idx)
            else:
                # -- a boundary
                cells[indices[0]].append(ridge_idx)

        # -- update remaining base class values
        self.num_nodes = len(vertices)
        self.coordinates_per_node = np.zeros([self.num_nodes, 2])
        for n, vertex in enumerate(vertices):
            self.coordinates_per_node[n, 0] = vertex[0]
            self.coordinates_per_node[n, 1] = vertex[1]
        self.num_faces_per_cell = np.zeros(self.num_cells, dtype=int)
        for n in range(self.num_cells):
            self.num_faces_per_cell[n] = len(cells[n])
        self.num_faces = sum(self.num_faces_per_cell)
        self.num_nodes_per_face = np.zeros(self.num_faces, dtype=int)
        for n in range(self.num_faces):
            self.num_nodes_per_face[n] = 2
        self.faces_per_cell = cells
        self.nodes_per_face = ridge_vertices

        # -- rewrite cells and faces to not have duplicate faces
        new_cells = []
        new_faces = []
        for cell in cells:
            new_cell = []
            for face in cell:
                new_faces.append(ridge_vertices[face])
                new_cell.append(len(new_faces) - 1)
            new_cells.append(new_cell)
        self.faces_per_cell = new_cells
        self.nodes_per_face = new_faces

        # -- write boundaries for new faces
        boundary_edges = {}
        boundary_edges['xl'] = []
        boundary_edges['xr'] = []
        boundary_edges['yl'] = []
        boundary_edges['yr'] = []
        for face_idx, v_indices in enumerate(self.nodes_per_face):
            midpoint = [(vertices[v_indices[0]][0] + vertices[v_indices[1]][0]) / 2,
                        (vertices[v_indices[0]][1] + vertices[v_indices[1]][1]) / 2]
            distances = np.zeros(len(cell_nodes))
            for n, node in enumerate(cell_nodes):
                distances[n] = np.sqrt((points[n][0] - midpoint[0])**2 +
                                       (points[n][1] - midpoint[1])**2)
            indices = np.argsort(distances)
            distances = np.sort(distances)
            if not soft_equiv(distances[0], distances[1]):
                # -- a boundary
                if (soft_equiv(vertices[v_indices[0]][0], xmin) and
                        soft_equiv(vertices[v_indices[1]][0], xmin)):
                    boundary_edges['xl'].append(face_idx)
                elif (soft_equiv(vertices[v_indices[0]][0], xmax) and
                      soft_equiv(vertices[v_indices[1]][0], xmax)):
                    boundary_edges['xr'].append(face_idx)
                elif (soft_equiv(vertices[v_indices[0]][1], ymin) and
                      soft_equiv(vertices[v_indices[1]][1], ymin)):
                    boundary_edges['yl'].append(face_idx)
                elif (soft_equiv(vertices[v_indices[0]][1], ymax) and
                      soft_equiv(vertices[v_indices[1]][1], ymax)):
                    boundary_edges['yr'].append(face_idx)
                else:
                    assert (False), 'Boundary edge not identified'
        self.nodes_per_side = []
        for n in range(4):
            bdy_key = list(boundary_edges.keys())[n]
            bdy_nodes = []
            for bdy in boundary_edges[bdy_key]:
                nodes = self.nodes_per_face[bdy]
                for node in nodes:
                    bdy_nodes.append(node)
            self.nodes_per_side.append(np.unique(bdy_nodes))


# ------------------------------------------------------------------------------------------------ #
# random 1D mesh type: orthogonal 1D mesh type with vertices randomly sampled in a ball
# centered at original (orthogonal) vertices
class rnd_1d_mesh(orth_1d_mesh):
    '''
    Class for mesh with randomly resampled vertices from orthogonally structured 1D mesh.
    The randomization currently only operates on interior vertices of the mesh.
    '''

    def __init__(self, bounds_per_dim, num_cells_per_dim, eps, rng_seed):
        assert (eps >= 0.0), 'eps < 0.0'
        assert (eps <= 1.0), 'eps > 1.0'
        # -- import pseudorandom number generator functions
        from random import seed, random
        # -- invoke base mesh constructor to generate initial mesh form
        orth_1d_mesh.__init__(self, bounds_per_dim, num_cells_per_dim)
        # -- short-cuts
        nx = num_cells_per_dim[0]
        # -- initial cell dimensions
        dx = (bounds_per_dim[0][1] - bounds_per_dim[0][0]) / nx
        # -- calculate radius of ball centered at original vertices
        r = 0.5 * dx * eps
        # -- set random number seed
        seed(rng_seed)
        # -- resample interior vertex coordinates
        for i in range(1, nx):
            node = i
            # -- sample displacements
            dx0 = r * (1.0 - 2.0 * random())
            # -- add displacement to coordinates
            self.coordinates_per_node[node, 0] += dx0


# ------------------------------------------------------------------------------------------------ #
# random 2D mesh type: orthogonal 2D mesh type with vertices randomly sampled in a ball
# centered at original (orthogonal) vertices
class rnd_2d_mesh(orth_2d_mesh):
    '''
    Class for mesh with randomly resampled vertices from orthogonally structured 2D mesh.
    The randomization currently only operates on interior vertices of the mesh.
    '''

    def __init__(self, bounds_per_dim, num_cells_per_dim, eps, rng_seed):
        assert (eps >= 0.0), 'eps < 0.0'
        assert (eps <= 1.0), 'eps > 1.0'
        # -- import pseudorandom number generator functions
        from random import seed, random
        # -- invoke base mesh constructor to generate initial mesh form
        orth_2d_mesh.__init__(self, bounds_per_dim, num_cells_per_dim)
        # -- short-cuts
        nx = num_cells_per_dim[0]
        ny = num_cells_per_dim[1]
        # -- initial cell dimensions
        dx = (bounds_per_dim[0][1] - bounds_per_dim[0][0]) / nx
        dy = (bounds_per_dim[1][1] - bounds_per_dim[1][0]) / ny
        # -- calculate radius of ball centered at original vertices
        r = 0.5 * min(dx, dy) * eps
        # -- set random number seed
        seed(rng_seed)
        # -- resample interior vertex coordinates
        for j in range(1, ny):
            j_offset = (nx + 1) * j
            for i in range(1, nx):
                node = i + j_offset
                # -- sample displacements
                dr = r * random()
                om = 2.0 * np.pi * random()
                dx0 = dr * np.cos(om)
                dy0 = dr * np.sin(om)
                # -- add displacement to coordinates
                self.coordinates_per_node[node, 0] += dx0
                self.coordinates_per_node[node, 1] += dy0


# ------------------------------------------------------------------------------------------------ #
# random 3D mesh type: face-centered cubic 3D mesh type with vertices randomly sampled in a ball
# centered at original (orthogonal) vertices
class rnd_3d_mesh(fcc_3d_mesh):
    '''
    Class for mesh with randomly resampled vertices from orthogonally structured 3D mesh.
    The randomization currently only operates on interior vertices of the mesh.
    '''

    def __init__(self, bounds_per_dim, num_cells_per_dim, eps, rng_seed):
        assert (eps >= 0.0), 'eps < 0.0'
        assert (eps <= 1.0), 'eps > 1.0'
        # -- import pseudorandom number generator functions
        from random import seed, random
        # -- invoke base mesh constructor to generate initial mesh form
        fcc_3d_mesh.__init__(self, bounds_per_dim, num_cells_per_dim)
        # -- short-cuts
        nx = num_cells_per_dim[0]
        ny = num_cells_per_dim[1]
        nz = num_cells_per_dim[2]
        # -- initial cell dimensions
        dx = (bounds_per_dim[0][1] - bounds_per_dim[0][0]) / nx
        dy = (bounds_per_dim[1][1] - bounds_per_dim[1][0]) / ny
        dz = (bounds_per_dim[2][1] - bounds_per_dim[2][0]) / nz
        # -- calculate radius of ball centered at original vertices
        r = 0.5 * min(dx, min(dy, dz)) * eps
        # -- set random number seed
        seed(rng_seed)
        # -- resample interior vertex coordinates
        for k in range(1, nz):
            k_offset = (nx + 1) * (ny + 1) * k
            for j in range(1, ny):
                j_offset = (nx + 1) * j
                for i in range(1, nx):
                    node = i + j_offset + k_offset
                    # -- sample displacements
                    dr = r * random()
                    mu = 1.0 - 2.0 * random()
                    om = 2.0 * np.pi * random()
                    xi = np.sqrt(1.0 - mu * mu)
                    dx0 = dr * mu
                    dy0 = dr * xi * np.cos(om)
                    dz0 = dr * xi * np.sin(om)
                    # -- add displacement to coordinates
                    self.coordinates_per_node[node, 0] += dx0
                    self.coordinates_per_node[node, 1] += dy0
                    self.coordinates_per_node[node, 2] += dz0

# ------------------------------------------------------------------------------------------------ #
# end of mesh_types.py
# ------------------------------------------------------------------------------------------------ #
