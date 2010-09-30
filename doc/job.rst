Main JOBMANAGER concept: Job
============================

This chapter explains the main JOBMANAGER concept: a job. 

What is a job ?
+++++++++++++++

A job is a work that a user want to perform on a computation resource (single computer or a cluster).
The JOBMANAGER different types of job depending of what is the user want to do.

There is three types of described in the table below.

======================== ==============================================================================
**Type of job**          **Description**
======================== ==============================================================================
**Command script**       It's a shell script containing the users commands. This kind of job is not 
                         related. It could be used to launch any codes.
**SALOME Python script** It's Python script that will be launched into a SALOME session dedicated to 
                         this script.
**YACS schema**          It's YACS schema that will be launched into a SALOME session dedicated to this
                         schema.
======================== ==============================================================================

Job content description
+++++++++++++++++++++++

All types of job share some attributes. There could be specific attribute for some types of job. It will
be indicated in this documentation if there is some specific attributes. A job has to kinds of attributes:
attributes that describes the job himself, and attributes that describes the computation requirements.

The first table below describes the attributes of the job.

======================== ================ ==============================================================================
**Attribute**            **Mandatory**    **Description**
======================== ================ ==============================================================================
**Name**                 Yes              This is the name of the job. It's unique into a SALOME session.
**Type**                 Yes              This is the type of the job. Currently there is three types: *command*,
                                          *python_salome* and *yacs_file*.
**Job file**             Yes              This is the name with the location of the file containing the job's data.
                                          Depending of the type it could a *shell* script, a *Python script* or
                                          a *YACS schema*.
**Env file**             No               An environnement file could be attached to the job. It will be executed before
                                          the job.
**Input files**          No               A list of files or directories in the user computer that have to copied into 
                                          the job's *work directory*.
**Output files**         No               A list of files or directories that have to be copied from the job's resource
                                          to the user computer into the *result directory*.
**Work directory**       No               It's the directory on the job's resource where the job will be executed. If
                                          the user does not provide a directory, a default directory will be created
                                          into the resource with this path: **~/Batch/"current date"**.
**Result directory**     Yes              It's the directory in the user computer where the job's results have to be 
                                          copied at the end of the job.
======================== ================ ==============================================================================

The second table below describes the attributes of computation requirements.

======================== ==============================================================================
**Attribute**            **Description**
======================== ==============================================================================
**Maximum duration**     It's maximum expected duration of the job. When a batch manager is used, this
                         time is interpreted as a **walltime** and not a **cputime**.
**Number of cpu**        It's the number of cpus/cores resquested.
**Memory**               It's the amoun of memory per cpu/core expected.
======================== ==============================================================================

Job's states
++++++++++++

A job could have many states in the JOBMANAGER. The table below describes the normal states.

======================== ==============================================================================
**State**                **Description**
======================== ==============================================================================
**Created**              The job is correctly created and could be launched.
**In_Process**           It's a transient state between *Created* and *Queued*.
**Queued**               The job is queued into the resource batch manager.
**Paused**               The job is paused. Currently the JOBMANAGER GUI does not allow to paused a 
                         job.
**Running**              The job is running on the resource.
**Finished**             The job has run an it's finished.
======================== ==============================================================================

The table below describes the errors states.

======================== ==============================================================================
**State**                **Description**
======================== ==============================================================================
**Not Created**          This state means that the job cannot be created with it's current description.
                         It's often a problem with the selected resource.
**Failed**               This state means that the execution of the job in the resource failed.
**Error**                This state is used when a job is loaded and that it cannot be followed. It
                         mainly happens when a job into a *ssh* resource is running. if the list is 
                         saved, it will an error when the list will be loaded. *ssh* resource cannot
                         be followed.
======================== ==============================================================================
