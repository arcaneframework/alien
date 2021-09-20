.. _user_concepts:

=========================
Alien's high level design
=========================

Alien's design key objective is to provide a robust interface for linear algebra users in the context of distributed
memory computations.

Robustness is achieved relying on well defined concepts to ensure a mathematically correct code, as well as a valid
parallel code.


Linear Algebra and Spaces
=========================

In Alien, we chose to view linear algebra objects as defined on generic indexed *spaces*.


Indeed, there are several ways to define a matrix, and we retain the following :
given three sets :math:`(I,J,K)`, a matrix :math:`M` is a family of elements of :math:`K`,
indexed by the cartesian product :math:`I \times J` (i.e. an application :math:`M : I \times J \mapsto K`.)

Most common indexed space are :math:`\{0,..,n-1\}` or :math:`\{1,..,n\}`.

Using named spaces allow us to ensure strict compatibility checks when computing.
For example, if :math:`u_I` and :math:`u_J` are two vectors defined on :math:`I` and :math:`J`,
we can detect that computing matrix-vector product :math:`M . u_I` is invalid while :math:`M . u_J` is valid.

If :math:`I` and :math:`J` have the same size, as it is the case for square matrices, it is impossible to distinguish
mathematically valid operations from invalid without using set's names.

Another advantage of using these named spaces is that we can unambiguously define sub-matrices.

Trilinos uses something similar but less formalized with its maps.


Keep user interface small
=========================


Extend functionalities: add new objects
=======================================
